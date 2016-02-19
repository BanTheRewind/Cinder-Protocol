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

BufferRef HeaderInterface::removeHeader( const BufferRef& buffer )
{
	string msg		= bufferToString( buffer );
	size_t offset	= msg.find( sCrLf + sCrLf );
	size_t sz		= buffer->getSize();
	if ( offset < sz ) {
		size_t len = ( sz - offset ) - 4;
		BufferRef body = Buffer::create( len );
		char_traits<char>::copy( (char*)body->getData(), (char*)buffer->getData() + ( offset + 4 ), len );
		return body;
	} else {
		return Buffer::create( 0 );
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

void HeaderInterface::parse( const BufferRef& buffer )
{
	string msg = bufferToString( buffer );
	parseHeader( msg );
}

BufferRef HeaderInterface::toBuffer() const
{
	string header	= headerToString();
	size_t sz		= header.size();
	
	BufferRef buffer = Buffer::create( sz );
	char_traits<char>::copy( (char*)buffer->getData(), (char*)&header[ 0 ], sz );
	
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
 