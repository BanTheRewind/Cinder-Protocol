#pragma once

#include "ProtocolInterface.h"

enum : size_t
{
	HTTP_0_9, HTTP_1_0, HTTP_1_1, HTTP_2_0
} typedef HttpVersion;

class HttpInterface : public ProtocolInterface
{
public:
	//! Creates HTTP message with version \a v.
	HttpInterface( HttpVersion v );
	
	//! Parses HTTP version from \a v.
	static HttpVersion	stringToHttpVersion( const std::string& v );
	//! Returns string representing HTTP version \a v.
	static std::string	httpVersionToString( HttpVersion v );
	
	//! Returns HTTP version.
	HttpVersion			getHttpVersion() const;
	//! Sets HTTP version to \a v.
	void				setHttpVersion( HttpVersion v );
	
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
	HttpVersion			mHttpVersion;
};
