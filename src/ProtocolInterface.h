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
	static KeyValuePair	stringToKeyValuePair( const std::string& kvp, const std::string& delim = " " );
	//! Returns string representation of key-value pair \a kvp.
	static std::string	keyValuePairToString( const KeyValuePair& kvp, const std::string& delim = " " );

	//! Return string \a value as Buffer.
	static ci::Buffer		stringToBuffer( const std::string& value );
	//! Returns string representation of \a buffer.
	static std::string		bufferToString( const ci::Buffer& buffer );
	
	virtual ci::Buffer		toBuffer() const = 0;
	virtual std::string		toString() const = 0;

	//! Parses \a msg into relevant commands, field, body, etc.
	void					parse( const std::string& msg );
	virtual void			parse( const ci::Buffer& buffer ) = 0;

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
 