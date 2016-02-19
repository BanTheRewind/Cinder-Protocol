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

#include "HttpRequest.h"

#include "boost/algorithm/string.hpp"
#include <vector>

using namespace ci;
using namespace std;

HttpRequest::HttpRequest()
: HttpInterface( HttpVersion::HTTP_1_1 ), mOptions( "GET" ), mUri( "*" )
{
}

HttpRequest::HttpRequest( const string& options, const string& uri, HttpVersion httpVersion )
: HttpInterface( httpVersion ), mOptions( options ), mUri( uri )
{
}

const string& HttpRequest::getOptions() const
{
	return mOptions;
}

void HttpRequest::setOptions( const string& options )
{
	mOptions = options;
}

const string& HttpRequest::getUri() const
{
	return mUri;
}

void HttpRequest::setUri( const string& uri )
{
	mUri = uri;
}

void HttpRequest::parseHeader( const string& header )
{
	if ( header.empty() ) {
		return;
	}
	size_t d	= header.find( sCrLf + sCrLf );
	size_t len	= header.length();
	string h	= header;
	if ( d < len ) {
		h		= h.substr( 0, d );
	}
	
	vector<string> lines;
	boost::split( lines, h, boost::is_any_of( sCrLf ) );
	if ( !lines.empty() ) {
		string request = boost::trim_copy( lines.at( 0 ) );
		vector<string> tokens;
		boost::split( tokens, request, boost::is_any_of( " " ) );
		if ( tokens.size() < 2 ) {
			throw ExcRequestLineInvalid( request );
		} else {
			mOptions		= tokens.at( 0 );
			mUri			= tokens.at( 1 );
			mHttpVersion	= stringToHttpVersion( tokens.at( 2 ) );
		}
		lines.erase( lines.begin() );
	}
	
	string headerMap	= boost::join( lines, sCrLf );
	mHeaderMap			= stringToHeaderMap( headerMap );
	
	mHasHeader			= true;
}

string HttpRequest::headerToString() const
{
	string header	= mOptions + " " + mUri + " ";
	header			+= httpVersionToString( mHttpVersion );
	header			+= sCrLf;
	if ( !mHeaderMap.empty() ) {
		header		+= headerMapToString( mHeaderMap );
	}
	header			+= sCrLf;
	return header;
}

HttpRequest::ExcRequestLineInvalid::ExcRequestLineInvalid( const string& requestLine ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid request line", requestLine.c_str() );
}
