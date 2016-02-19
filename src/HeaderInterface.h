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

#include "ProtocolInterface.h"

typedef std::map<std::string, std::string> HeaderMap;

class HeaderInterface : public ProtocolInterface
{
public:
	//! Parses \a headerMap into a header map object.
	static HeaderMap		stringToHeaderMap( const std::string& headerMap );
	//! Return string representation of \a headerMap.
	static std::string		headerMapToString( const HeaderMap& headerMap );
	//! Returns copy of buffer with header removed.
	static ci::BufferRef	removeHeader( const ci::BufferRef& buffer );

	//! Erases header named \a field.
	void					eraseHeader( const std::string& field );
	//! Returns header value for \a field.
	const std::string&		getHeader( const std::string& field );
	//! Return header map.
	HeaderMap&				getHeaders();
	//! Return header map.
	const HeaderMap&		getHeaders() const;
	//! Returns true if a valid header exists.
	bool					hasHeader() const;
	//! Set header \a value for \a field. Overwrites existing value with same key.
	void					setHeader( const std::string& field, const std::string& value );
	//! Set header field from \a kvp. Overwrites existing value with same key.
	void					setHeader( const KeyValuePair& kvp );
	// Sets all header fields from \a headerMap. Overwrites existing values.
	void					setHeaders( const HeaderMap& headerMap );
	//! Parses \a buffer into headers.
	virtual void			parse( const ci::BufferRef& buffer );
	virtual void			parseHeader( const std::string& header ) = 0;

	//! Converts entire message to ci::Buffer.
	virtual ci::BufferRef	toBuffer() const;
	//! Converts entire message to std::string.
	virtual std::string		toString() const;
	
	//! Exception representing missing header
	class ExcHeaderNotFound : public ci::Exception
	{
	public:
		ExcHeaderNotFound( const std::string& field ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	HeaderInterface();
	
	virtual std::string		headerToString() const = 0;

	bool					mHasHeader;
	HeaderMap				mHeaderMap;
};

  