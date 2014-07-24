#include "ProtocolInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

string ProtocolInterface::sCrLf	= "\r\n";

ProtocolInterface::ProtocolInterface()
{
}

KeyValuePair ProtocolInterface::stringToKeyValuePair( const string& kvp, const string& delim )
{
	vector<string> tokens;
	boost::split( tokens, kvp, boost::is_any_of( delim ) );
	if ( tokens.size() < 2 ) {
		throw ExcKeyValuePairInvalid( kvp );
	} else {
		string key		= boost::trim_copy( tokens.at( 0 ) );
		tokens.erase( tokens.begin() );
		string value	= boost::join( tokens, delim );
		value			= boost::trim_copy( value );
	
		KeyValuePair kvp( key, value );
		return kvp;
	}
}

string ProtocolInterface::keyValuePairToString( const KeyValuePair& kvp, const string& delim )
{
	return kvp.first + delim + kvp.second;
}

Buffer ProtocolInterface::stringToBuffer( const string& value )
{
	return Buffer( (char*)&value[ 0 ], value.size() );
}

string ProtocolInterface::bufferToString( const Buffer& buffer )
{
	string s( static_cast<const char*>( buffer.getData() ) );
	if ( s.length() > buffer.getDataSize() ) {
		s.resize( buffer.getDataSize() );
	}
	return s;
}

void ProtocolInterface::parse( const string& msg )
{
	Buffer buffer( (char*)&msg[ 0 ], msg.length() );
	parse( buffer );
}

ProtocolInterface::ExcKeyValuePairInvalid::ExcKeyValuePairInvalid( const string& kvp ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid key:value pair", kvp.c_str() );
}
