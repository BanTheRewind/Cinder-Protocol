#include "ProtocolInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

string ProtocolInterface::sCrLf	= "\r\n";

ProtocolInterface::ProtocolInterface()
: mHasHeader( false )
{
}

Buffer ProtocolInterface::stringToBuffer( string& value )
{
	return Buffer( &value[ 0 ], value.size() );
}

string ProtocolInterface::bufferToString( const Buffer& buffer )
{
	return string( static_cast<const char*>( buffer.getData() ), buffer.getDataSize() );
}

HeaderMap ProtocolInterface::stringToHeaderMap( const string& h )
{
	vector<string> tokens;
	boost::split( tokens, h, boost::is_any_of( sCrLf ) );
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
		headerMap += keyValuePairToString( *iter ) + sCrLf;
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

Buffer ProtocolInterface::removeHeader( const Buffer& buffer )
{
	string msg		= bufferToString( buffer );
	size_t offset	= msg.find( sCrLf + sCrLf );
	size_t sz		= buffer.getDataSize();
	if ( offset < sz ) {
		size_t len = ( sz - offset ) - 4;
		Buffer body( len );
		char_traits<char>::copy( (char*)body.getData(), (char*)buffer.getData() + ( offset + 4 ), len );
		return body;
	} else {
		return Buffer();
	}
}

void ProtocolInterface::append( const ci::Buffer& buffer )
{
	size_t sz	= 0;
	size_t len	= buffer.getDataSize();
	if ( mBody ) {
		sz = mBody.getDataSize();
		mBody.resize( sz + len );
	} else {
		mBody = Buffer( len );
	}
	char_traits<char>::copy( (char*)mBody.getData() + sz, (char*)buffer.getData(), len );
}

const Buffer& ProtocolInterface::getBody() const
{
	return mBody;
}

void ProtocolInterface::setBody( const Buffer& body )
{
	size_t sz = body.getDataSize();
	if ( mBody && sz > 0 ) {
		mBody.resize( sz );
	} else {
		mBody = Buffer( sz );
	}
	char_traits<char>::copy( (char*)mBody.getData(), (char*)body.getData(), sz );
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

bool ProtocolInterface::hasHeader() const
{
	return mHasHeader;
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

void ProtocolInterface::parse( const string& msg )
{
	vector<string> tokens;
	boost::split( tokens, msg, boost::is_any_of( sCrLf + sCrLf ) );
	string body = "";
	for ( size_t i = 0; i < tokens.size(); ++i ) {
		const string& v = tokens.at( i );
		if ( i == 0 ) {
			parseHeader( v );
		} else {
			body += v;
		}
	}
	mBody = stringToBuffer( body );
}

void ProtocolInterface::parse( const Buffer& buffer )
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
	} else {
		parse( msg );
	}
}

Buffer ProtocolInterface::toBuffer() const
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

string ProtocolInterface::toString() const
{
	string body		= "";
	string header	= headerToString();
	if ( mBody ) {
		body		= bufferToString( mBody );
	}
	return header + body;
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
