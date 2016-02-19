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

#include "HttpResponse.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"

using namespace ci;
using namespace std;

HttpResponse::HttpResponse()
: HttpInterface( HttpVersion::HTTP_1_1 ), mReason( "" ), mStatusCode( 0 )
{
}

HttpResponse::HttpResponse( HttpVersion httpVersion, size_t statusCode, const string& reason )
: HttpInterface( HttpVersion::HTTP_1_1 ), mReason( reason ), mStatusCode( statusCode )
{
}

const string& HttpResponse::getReason() const
{
	return mReason;
}

void HttpResponse::setReason( const string& reason )
{
	mReason = reason;
}

size_t HttpResponse::getStatusCode() const
{
	return mStatusCode;
}

void HttpResponse::setStatusCode( size_t code )
{
	mStatusCode = code;
}

void HttpResponse::parseHeader( const string& header )
{
	if ( header.empty() ) {
		return;
	}
	size_t d	= header.find( sCrLf + sCrLf );
	size_t l	= header.length();
	string h	= header;
	if ( d < l ) {
		h		= h.substr( 0, d );
	}
	
	vector<string> lines;
	boost::split( lines, h, boost::is_any_of( sCrLf ) );
	if ( !lines.empty() ) {
		string status = boost::trim_copy( lines.at( 0 ) );
		vector<string> tokens;
		boost::split( tokens, status, boost::is_any_of( " " ) );
		if ( tokens.size() < 2 ) {
			throw ExcStatusLineInvalid( status );
		} else {
			mHttpVersion	= stringToHttpVersion( tokens.at( 0 ) );
			mStatusCode		= fromString<size_t>( tokens.at( 1 ) );
			mReason			= "";
			for ( size_t i = 0; i < 2; ++i ) {
				tokens.erase( tokens.begin() );
			}
			if ( !tokens.empty() ) {
				mReason		= boost::join( tokens, " " );
			}
		}
		lines.erase( lines.begin() );
	}
	
	string headerMap	= boost::join( lines, sCrLf );
	mHeaderMap			= stringToHeaderMap( headerMap );
	mHasHeader			= true;
}

string HttpResponse::headerToString() const
{
	string header	= httpVersionToString( mHttpVersion );;
	header			+= " " + ci::toString( mStatusCode ) + " " + mReason;
	header			+= sCrLf;
	if ( !mHeaderMap.empty() ) {
		header		+= headerMapToString( mHeaderMap );
	}
	header			+= sCrLf;
	return header;
}

HttpResponse::ExcStatusLineInvalid::ExcStatusLineInvalid( const string& statusLine ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid status line", statusLine.c_str() );
}
