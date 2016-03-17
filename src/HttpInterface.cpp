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

void HttpInterface::parse( const BufferRef& buffer )
{
	string msg		= bufferToString( buffer );
	size_t offset	= msg.find( sCrLf + sCrLf );
	size_t sz		= buffer->getSize();
	if ( offset < sz ) {
		msg = msg.substr( 0, offset );
		parseHeader( msg );

		size_t len		= sz - offset;
		BufferRef body	= Buffer::create( len );
		char_traits<char>::copy( (char*)body->getData(), (char*)buffer->getData() + ( offset + 4 ), len );
		mBody = body;
	}
}

BufferRef HttpInterface::toBuffer() const
{
	string header		= headerToString();
	size_t headerLength	= header.size();
	size_t bodyLength	= 0;
	if ( mBody ) {
		bodyLength		= mBody->getSize();
	}
	BufferRef buffer = Buffer::create( headerLength + bodyLength );
	char_traits<char>::copy( (char*)buffer->getData(), (char*)&header[ 0 ], headerLength );
	if ( bodyLength > 0 ) {
		char_traits<char>::copy( (char*)buffer->getData() + headerLength, (char*)mBody->getData(), bodyLength );
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
 