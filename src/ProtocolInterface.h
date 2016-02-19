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

#include "cinder/Buffer.h"
#include "cinder/Exception.h"
#include <map>
#include <string>
#include <ostream>

typedef std::pair<std::string, std::string>	KeyValuePair;

class ProtocolInterface
{
public:
	//! Parses \a kvp into a key-value pair object.
	static KeyValuePair		stringToKeyValuePair( const std::string& kvp, const std::string& delim = " " );
	//! Returns string representation of key-value pair \a kvp.
	static std::string		keyValuePairToString( const KeyValuePair& kvp, const std::string& delim = " " );

	//! Return string \a value as Buffer.
	static ci::BufferRef	stringToBuffer( const std::string& value );
	//! Returns string representation of \a buffer.
	static std::string		bufferToString( const ci::BufferRef& buffer );
	
	virtual ci::BufferRef	toBuffer() const = 0;
	virtual std::string		toString() const = 0;

	//! Parses \a msg into relevant commands, field, body, etc.
	void					parse( const std::string& msg );
	virtual void			parse( const ci::BufferRef& buffer ) = 0;

	//! Exception representing invalid key-value pair
	class ExcKeyValuePairInvalid : public ci::Exception
	{
	public:
		ExcKeyValuePairInvalid( const std::string& kvp ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	ProtocolInterface();
	
	static std::string		sCrLf;
};
 