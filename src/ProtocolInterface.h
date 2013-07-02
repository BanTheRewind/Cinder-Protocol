#pragma once

#include "cinder/Buffer.h"
#include "cinder/Exception.h"
#include <map>
#include <string>
#include <ostream>

typedef std::pair<std::string, std::string>	KeyValuePair;
typedef std::map<std::string, std::string>	HeaderMap;

class ProtocolInterface
{
public:
	ProtocolInterface();
	
	static ci::Buffer		stringToBuffer( std::string& value );
	static std::string		bufferToString( const ci::Buffer& buffer );
	
	static HeaderMap		stringToHeaderMap( const std::string& h );
	static std::string		headerMapToString( const HeaderMap& h );
	
	static KeyValuePair		stringToKeyValuePair( const std::string& kvp );
	static std::string		keyValuePairToString( const KeyValuePair& kvp );
	
	const ci::Buffer&		getBody() const;
	void					setBody( const ci::Buffer& body );
	
	void					eraseHeader( const std::string& field );
	const std::string&		getHeader( const std::string& field );
	HeaderMap&				getHeaders();
	const HeaderMap&		getHeaders() const;
	void					setHeader( const std::string& field, const std::string& value );
	void					setHeader( const KeyValuePair& kvp );
	void					setHeaders( const HeaderMap& headerMap );
	
	virtual ci::Buffer		toBuffer() const;
	virtual std::string		toString() const;
	
	class ExcHeaderNotFound : public ci::Exception
	{
	public:
		ExcHeaderNotFound( const std::string &field ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
	
	class ExcKeyValuePairInvalid : public ci::Exception
	{
	public:
		ExcKeyValuePairInvalid( const std::string &kvp ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	static std::string		sCrLf;
	static std::string		sLf;
	
	virtual std::string		headerToString() const = 0;
	
	ci::Buffer				mBody;
	HeaderMap				mHeaderMap;
	
	friend std::ostream&	operator<<( std::ostream& out, const ProtocolInterface& h );
};

std::ostream&				operator<<( std::ostream& out, const ProtocolInterface& h );
