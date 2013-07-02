#pragma once

#include "HttpInterface.h"

class HttpRequest : public HttpInterface
{
public:
	HttpRequest();
	HttpRequest( const std::string& options, const std::string& uri, HttpVersion httpVersion );
	
	const std::string&	getOptions() const;
	void				setOptions( const std::string& options );

	const std::string&	getUri() const;
	void				setUri( const std::string& uri );
	
	void				parseRequestLine( const std::string& requestLine );
	
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
	std::string			headerToString() const;
	
	std::string			mOptions;
	std::string			mUri;
};
