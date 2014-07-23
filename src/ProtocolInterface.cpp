#include "ProtocolInterface.h"

using namespace ci;
using namespace std;

string ProtocolInterface::sCrLf	= "\r\n";

ProtocolInterface::ProtocolInterface()
{
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
