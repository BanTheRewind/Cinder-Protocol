#pragma once

#include "ProtocolInterface.h"

enum : size_t
{
	HTTP_0_9, HTTP_1_0, HTTP_1_1, HTTP_2_0
} typedef HttpVersion;

class HttpInterface : public ProtocolInterface
{
public:
	HttpInterface( HttpVersion v );
	
	static HttpVersion	stringToHttpVersion( const std::string& v );
	static std::string	httpVersionToString( HttpVersion v );
	
	HttpVersion			getHttpVersion() const;
	void				setHttpVersion( HttpVersion v );
	
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
	HttpVersion			mHttpVersion;
};
