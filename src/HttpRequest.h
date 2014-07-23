#pragma once

#include "HttpInterface.h"

class HttpRequest : public HttpInterface
{
public:
	//! Creates empty HTTP request.
	HttpRequest();
	//! Creates a HTTP request. Populates request line with arguments.
	HttpRequest( const std::string& options, const std::string& uri, HttpVersion httpVersion );
	
	//! Returns HTTP options as string.
	const std::string&	getOptions() const;
	//! Sets HTTP options to \a options.
	void		setOptions( const std::string& options );

	//! Return URI as string.
	const std::string&	getUri() const;
	//! Sets URI to \a uri.
	void		setUri( const std::string& uri );
	
	/*! Parses header from \a header. Throws exceptions for incomplete
		or invalid headers. */
	void		parseHeader( const std::string& header );
	
	//! Exception representing invalid request line.
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
	std::string	headerToString() const;
	
	std::string	mOptions;
	std::string	mUri;
};
