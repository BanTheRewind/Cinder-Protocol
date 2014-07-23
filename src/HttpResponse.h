#pragma once

#include "HttpInterface.h"

class HttpResponse : public HttpInterface
{
public:
	//! Creates empty HTTP response.
	HttpResponse();
	//! Creates a HTTP response. Populates status line with arguments.
	HttpResponse( HttpVersion httpVersion, size_t statusCode, const std::string& reason );
	
	//! Returns reason.
	const std::string&	getReason() const;
	//! Sets reason to \a reason.
	void		setReason( const std::string& reason );
	
	//! Returns status code.
	size_t		getStatusCode() const;
	//! Sets status code to \a code.
	void		setStatusCode( size_t code );
	
	/*! Parses header from \a header. Throws exceptions for incomplete
		or invalid headers. */
	void		parseHeader( const std::string& header );
	
	//! Exception representing invalid status line.
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
	std::string	headerToString() const;
	
	std::string	mReason;
	size_t		mStatusCode;
};
