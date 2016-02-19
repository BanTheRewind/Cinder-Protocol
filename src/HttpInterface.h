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

#include "BodyInterface.h"
#include "HeaderInterface.h"

enum : size_t
{
	HTTP_0_9, HTTP_1_0, HTTP_1_1, HTTP_2_0
} typedef HttpVersion;

class HttpInterface : public BodyInterface, public HeaderInterface
{
public:
	//! Parses HTTP version from \a v.
	static HttpVersion		stringToHttpVersion( const std::string& v );
	//! Returns string representing HTTP version \a v.
	static std::string		httpVersionToString( HttpVersion v );
	
	//! Returns HTTP version.
	HttpVersion				getHttpVersion() const;
	//! Sets HTTP version to \a v.
	void					setHttpVersion( HttpVersion v );

	//! Parses \a buffer to header and body. Throws exception for invalid header.
	void					parse( const ci::BufferRef& buffer );

	//! Converts entire message to ci::Buffer.
	ci::BufferRef			toBuffer() const;
	//! Converts entire message to std::string.
	std::string				toString() const;

	//! Exception representing invalid HTTP version.
	class ExcHttpVersionInvalid : public ci::Exception
	{
	public:
		ExcHttpVersionInvalid( const std::string &ver ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:	
	HttpInterface( HttpVersion v );

	HttpVersion				mHttpVersion;

	friend std::ostream&	operator<<( std::ostream& out, const HttpInterface& h );
};

std::ostream&				operator<<( std::ostream& out, const HttpInterface& h );
 