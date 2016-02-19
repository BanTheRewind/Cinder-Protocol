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

#pragma once

#include "HttpInterface.h"

class HttpRequest : public HttpInterface
{
public:
	//! Creates empty HTTP request.
	HttpRequest();
	//! Creates a HTTP request. Populates request line with arguments.
	HttpRequest( const std::string& options, const std::string& uri, HttpVersion httpVersion );
	
	//! Returns HTTP options as string.
	const std::string&	getOptions() const;
	//! Sets HTTP options to \a options.
	void		setOptions( const std::string& options );

	//! Return URI as string.
	const std::string&	getUri() const;
	//! Sets URI to \a uri.
	void		setUri( const std::string& uri );
	
	/*! Parses header from \a header. Throws exceptions for incomplete
		or invalid headers. */
	void		parseHeader( const std::string& header );
	
	//! Exception representing invalid request line.
	class ExcRequestLineInvalid : public ci::Exception
	{
	public:
		ExcRequestLineInvalid( const std::string &requestLine ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	std::string	headerToString() const;
	
	std::string	mOptions;
	std::string	mUri;
};
