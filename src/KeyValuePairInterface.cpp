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

#include "KeyValuePairInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

KeyValuePairInterface::KeyValuePairInterface( const string& key, const string& value )
: ProtocolInterface(), mKeyValuePair( make_pair( key, value ) )
{
}

const string& KeyValuePairInterface::getValue() const
{
	return mKeyValuePair.second;
}

void KeyValuePairInterface::setValue( const string& v )
{
	mKeyValuePair.second = v;
}

void KeyValuePairInterface::parse( const BufferRef& buffer )
{
	string s = bufferToString( buffer );
	vector<string> lines;
	boost::split( lines, s, boost::is_any_of( sCrLf ) );
	if ( !lines.empty() ) {
		vector<string> pair;
		boost::split( pair, lines.at( 0 ), boost::is_any_of( " " ) );
		if ( pair.size() > 1 ) {
			string key		= pair.at( 0 );
			pair.erase( pair.begin() );
			string value	= boost::join( pair, " " );
			mKeyValuePair	= make_pair( key, value );
		}
	}
}

BufferRef KeyValuePairInterface::toBuffer() const
{
	return stringToBuffer( toString() );
}

string KeyValuePairInterface::toString() const
{
	return keyValuePairToString( mKeyValuePair );
}

ostream& operator<<( ostream& out, const KeyValuePairInterface& p )
{
	out << p.toString();
	return out;
}
 