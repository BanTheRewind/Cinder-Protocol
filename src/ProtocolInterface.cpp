#include "ProtocolInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

string ProtocolInterface::sCrLf	= "\r\n";
string ProtocolInterface::sLf	= "\n";

ProtocolInterface::ProtocolInterface()
{
}

Buffer ProtocolInterface::stringToBuffer( string& value )
{
	return Buffer( &value[ 0 ], value.size() );
}

string ProtocolInterface::bufferToString( const Buffer& buffer )
{
	return string( static_cast<const char*>( buffer.getData() ) );
}

HeaderMap ProtocolInterface::stringToHeaderMap( const string& h )
{
	vector<string> tokens;
	boost::split( tokens, h, boost::is_any_of( sLf ) );
	HeaderMap headerMap;
	for ( vector<string>::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter ) {
		if ( !iter->empty() ) {
			KeyValuePair kvp = stringToKeyValuePair( *iter );
			headerMap.insert( kvp );
		}
	}
	return headerMap;
}

string ProtocolInterface::headerMapToString( const HeaderMap& h )
{
	string headerMap = "";
	for ( HeaderMap::const_iterator iter = h.begin(); iter != h.end(); ++iter ) {
		headerMap += keyValuePairToString( *iter ) + sLf;
	}
	return headerMap;
}

KeyValuePair ProtocolInterface::stringToKeyValuePair( const string& kvp )
{
	vector<string> tokens;
	boost::split( tokens, kvp, boost::is_any_of( ":" ) );
	if ( tokens.size() < 2 ) {
		throw ExcKeyValuePairInvalid( kvp );
	} else {
		string key		= boost::trim_copy( tokens.at( 0 ) );
		tokens.erase( tokens.begin() );
		string value	= boost::join( tokens, ":" );
		value			= boost::trim_copy( value );
	
		KeyValuePair kvp( key, value );
		return kvp;
	}
}

string ProtocolInterface::keyValuePairToString( const KeyValuePair& kvp )
{
	return kvp.first + ": " + kvp.second;
}

void ProtocolInterface::append( const ci::Buffer& buffer )
{
	if ( mBody ) {
		size_t s0 = mBody.getDataSize();
		size_t s1 = buffer.getDataSize();
		ci::Buffer b( new char[ s0 + s1 ], s0 + s1 );
		char_traits<char>::copy( (char*)b.getData(),		(char*)mBody.getData(),		s0 );
		char_traits<char>::copy( (char*)b.getData() + s0,	(char*)buffer.getData(),	s1 );
		mBody = b;
	} else {
		mBody = buffer;
	}
}

const Buffer& ProtocolInterface::getBody() const
{
	return mBody;
}

void ProtocolInterface::setBody( const Buffer& body )
{
	mBody = body;
}

void ProtocolInterface::eraseHeader( const string& field )
{
	if ( mHeaderMap.find( field ) == mHeaderMap.end() ) {
		throw ExcHeaderNotFound( field );
	} else {
		mHeaderMap.erase( field );
	}
}

const string& ProtocolInterface::getHeader( const string& field )
{
	if ( mHeaderMap.find( field ) == mHeaderMap.end() ) {
		throw ExcHeaderNotFound( field );
	} else {
		return mHeaderMap[ field ];
	}
}

HeaderMap& ProtocolInterface::getHeaders()
{
	return mHeaderMap;
}

const HeaderMap& ProtocolInterface::getHeaders() const
{
	return mHeaderMap;
}

void ProtocolInterface::setHeader( const string& field, const string& value )
{
	mHeaderMap[ field ] = value;
}

void ProtocolInterface::setHeader( const KeyValuePair& kvp )
{
	setHeader( kvp.first, kvp.second );
}

void ProtocolInterface::setHeaders( const HeaderMap& headerMap )
{
	mHeaderMap = headerMap;
}

ci::Buffer ProtocolInterface::toBuffer() const
{
	string header		= headerToString();
	size_t headerLength	= header.size();
	size_t bodyLength	= 0;
	if ( mBody ) {
		bodyLength		= mBody.getDataSize() + 2;
	}
	
	size_t pos			= 0;
	size_t total		= headerLength + bodyLength + 2;
	
	Buffer buffer( new char[ total ], total );
	
	memcpy( (char*)buffer.getData() + pos,		&header[ 0 ],		headerLength );
	pos					+= headerLength;
	memcpy( (char*)buffer.getData() + pos,		&sCrLf[ 0 ],		2 );
	pos					+= 2;
	if ( bodyLength > 0 ) {
		memcpy( (char*)buffer.getData() + pos,	mBody.getData(),	bodyLength );
		pos				+= bodyLength;
		memcpy( (char*)buffer.getData() + pos,	&sCrLf[ 0 ],		2 );
	}
	
	return buffer;
}

string ProtocolInterface::toString() const
{
	string request	= headerToString();
	if ( mBody ) {
		request		+= sCrLf;
		request		+= bufferToString( mBody );
	}
	return request;
}

ProtocolInterface::ExcHeaderNotFound::ExcHeaderNotFound( const string& field ) throw()
{
	sprintf( mMessage, "Header field \"%s\" not found", field.c_str() );
}

ProtocolInterface::ExcKeyValuePairInvalid::ExcKeyValuePairInvalid( const string& kvp ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid key:value pair", kvp.c_str() );
}

ostream& operator<<( ostream& out, const ProtocolInterface& p )
{
	out << p.toString();
	return out;
}
