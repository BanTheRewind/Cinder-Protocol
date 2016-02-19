/*
* 
* Copyright (c) 2016, Wieden+Kennedy, 
* Stephen Schieberl
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or 
* without modification, are permitted provided that the following 
* conditions are met:
* 
* Redistributions of source code must retain the above copyright 
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright 
* notice, this list of conditions and the following disclaimer in 
* the documentation and/or other materials provided with the 
* distribution.
* 
* Neither the name of the Ban the Rewind nor the names of its 
* contributors may be used to endorse or promote products 
* derived from this software without specific prior written 
* permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
*/

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

BufferRef ProtocolInterface::stringToBuffer( const string& value )
{
	BufferRef buffer = Buffer::create( value.size() );
	buffer->copyFrom( (char*)&value[ 0 ], value.size() );
	return buffer;
}

string ProtocolInterface::bufferToString( const BufferRef& buffer )
{
	string s( static_cast<const char*>( buffer->getData() ) );
	if ( s.length() > buffer->getSize() ) {
		s.resize( buffer->getSize() );
	}
	return s;
}

void ProtocolInterface::parse( const string& msg )
{
	if ( !msg.empty() ) { 
		parse( Buffer::create( (char*)&msg[ 0 ], msg.length() ) );
	}
}

ProtocolInterface::ExcKeyValuePairInvalid::ExcKeyValuePairInvalid( const string& kvp ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid key:value pair", kvp.c_str() );
}
