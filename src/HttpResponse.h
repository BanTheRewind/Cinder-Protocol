#pragma once

#include "HttpInterface.h"

class HttpResponse : public HttpInterface
{
public:
	HttpResponse();
	HttpResponse( HttpVersion httpVersion, size_t statusCode, const std::string& reason );
	
	const std::string&	getReason() const;
	void				setReason( const std::string& reason );
	
	size_t				getStatusCode() const;
	void				setStatusCode( size_t code );
	
	void				parseHeader( const std::string& header );
		
	class ExcStatusLineInvalid : public ci::Exception
	{
	public:
		ExcStatusLineInvalid( const std::string &statusLine ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	std::string			headerToString() const;
	
	std::string			mReason;
	size_t				mStatusCode;
};
