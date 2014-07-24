#include "HeaderInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

HeaderInterface::HeaderInterface()
: ProtocolInterface(), mHasHeader( false )
{
}

HeaderMap HeaderInterface::stringToHeaderMap( const string& h )
{
	vector<string> tokens;
	boost::split( tokens, h, boost::is_any_of( sCrLf ) );
	HeaderMap headerMap;
	for ( vector<string>::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter ) {
		if ( !iter->empty() ) {
			KeyValuePair kvp = stringToKeyValuePair( *iter, ":" );
			headerMap.insert( kvp );
		}
	}
	return headerMap;
}

string HeaderInterface::headerMapToString( const HeaderMap& h )
{
	string headerMap = "";
	for ( HeaderMap::const_iterator iter = h.begin(); iter != h.end(); ++iter ) {
		headerMap += keyValuePairToString( *iter, ":" ) + sCrLf;
	}
	return headerMap;
}

Buffer HeaderInterface::removeHeader( const Buffer& buffer )
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

void HeaderInterface::eraseHeader( const string& field )
{
	if ( mHeaderMap.find( field ) == mHeaderMap.end() ) {
		throw ExcHeaderNotFound( field );
	} else {
		mHeaderMap.erase( field );
	}
}

const string& HeaderInterface::getHeader( const string& field )
{
	if ( mHeaderMap.find( field ) == mHeaderMap.end() ) {
		throw ExcHeaderNotFound( field );
	} else {
		return mHeaderMap[ field ];
	}
}

HeaderMap& HeaderInterface::getHeaders()
{
	return mHeaderMap;
}

const HeaderMap& HeaderInterface::getHeaders() const
{
	return mHeaderMap;
}

bool HeaderInterface::hasHeader() const
{
	return mHasHeader;
}

void HeaderInterface::setHeader( const string& field, const string& value )
{
	mHeaderMap[ field ] = value;
}

void HeaderInterface::setHeader( const KeyValuePair& kvp )
{
	setHeader( kvp.first, kvp.second );
}

void HeaderInterface::setHeaders( const HeaderMap& headerMap )
{
	mHeaderMap = headerMap;
}

void HeaderInterface::parse( const Buffer& buffer )
{
	string msg		= bufferToString( buffer );
	parseHeader( msg );
}

Buffer HeaderInterface::toBuffer() const
{
	string header	= headerToString();
	size_t sz		= header.size();
	
	Buffer buffer( sz );
	char_traits<char>::copy( (char*)buffer.getData(), (char*)&header[ 0 ], sz );
	
	return buffer;
}

string HeaderInterface::toString() const
{
	return headerToString();
}

HeaderInterface::ExcHeaderNotFound::ExcHeaderNotFound( const string& field ) throw()
{
	sprintf( mMessage, "Header field \"%s\" not found", field.c_str() );
}
 