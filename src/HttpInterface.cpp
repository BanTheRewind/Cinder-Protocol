#include "HttpInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

HttpInterface::HttpInterface( HttpVersion v )
: BodyInterface(), HeaderInterface(), mHttpVersion( v )
{
}

HttpVersion HttpInterface::stringToHttpVersion( const string& v )
{
	vector<string> tokens;
	boost::split( tokens, v, boost::is_any_of( "/" ) );
	if ( tokens.size() == 2 ) {
		string ver = tokens.at( 1 );
		if ( ver == "0.9" ) {
			return HTTP_0_9;
		} else if ( ver == "1.0" ) {
			return HTTP_1_0;
		} else if ( ver == "1.1" ) {
			return HTTP_1_1;
		} else if ( ver == "2.0" ) {
			return HTTP_2_0;
		} else {
			throw ExcHttpVersionInvalid( v );
		}
	} else {
		throw ExcHttpVersionInvalid( v );
	}
}

string HttpInterface::httpVersionToString( HttpVersion v )
{
	static const string prefix = "HTTP/";
	switch ( v ) {
		case HTTP_0_9:
			return prefix + "0.9";
		case HTTP_1_0:
			return prefix + "1.0";
		case HTTP_1_1:
			return prefix + "1.1";
		case HTTP_2_0:
			return prefix + "2.0";
		default:
			throw ExcHttpVersionInvalid( ci::toString( v ) );
			break;
	}
}

HttpVersion HttpInterface::getHttpVersion() const
{
	return mHttpVersion;
}

void HttpInterface::setHttpVersion( HttpVersion v )
{
	mHttpVersion = v;
}

void HttpInterface::parse( const Buffer& buffer )
{
	string msg		= bufferToString( buffer );
	size_t offset	= msg.find( sCrLf + sCrLf );
	size_t sz		= buffer.getDataSize();
	if ( offset < sz ) {
		msg = msg.substr( 0, offset );
		parseHeader( msg );

		size_t len = sz - offset;
		Buffer body( len );
		char_traits<char>::copy( (char*)body.getData(), (char*)buffer.getData() + ( offset + 4 ), len );
		mBody = body;
	}
}

Buffer HttpInterface::toBuffer() const
{
	string header		= headerToString();
	size_t headerLength	= header.size();
	size_t bodyLength	= 0;
	if ( mBody ) {
		bodyLength		= mBody.getDataSize();
	}
	size_t sz			= headerLength + bodyLength;

	Buffer buffer( sz );
	char_traits<char>::copy( (char*)buffer.getData(), (char*)&header[ 0 ], headerLength );
	if ( bodyLength > 0 ) {
		char_traits<char>::copy( (char*)buffer.getData() + headerLength, (char*)mBody.getData(), bodyLength );
	}

	return buffer;
}

string HttpInterface::toString() const
{
	string body		= "";
	string header	= headerToString();
	if ( mBody ) {
		body		= bufferToString( mBody );
	}
	return header + body;
}


HttpInterface::ExcHttpVersionInvalid::ExcHttpVersionInvalid( const string& ver ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid HTTP version", ver.c_str() );
}

ostream& operator<<( ostream& out, const HttpInterface& p )
{
	out << p.toString();
	return out;
}
 