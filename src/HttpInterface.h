#pragma once

#include "BodyInterface.h"
#include "HeaderInterface.h"

enum : size_t
{
	HTTP_0_9, HTTP_1_0, HTTP_1_1, HTTP_2_0
} typedef HttpVersion;

class HttpInterface : public BodyInterface, public HeaderInterface
{
public:
	//! Parses HTTP version from \a v.
	static HttpVersion		stringToHttpVersion( const std::string& v );
	//! Returns string representing HTTP version \a v.
	static std::string		httpVersionToString( HttpVersion v );
	
	//! Returns HTTP version.
	HttpVersion				getHttpVersion() const;
	//! Sets HTTP version to \a v.
	void					setHttpVersion( HttpVersion v );

	//! Parses \a buffer to header and body. Throws exception for invalid header.
	void					parse( const ci::Buffer& buffer );

	//! Converts entire message to ci::Buffer.
	ci::Buffer				toBuffer() const;
	//! Converts entire message to std::string.
	std::string				toString() const;

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
	HttpInterface( HttpVersion v );

	HttpVersion				mHttpVersion;

	friend std::ostream&	operator<<( std::ostream& out, const HttpInterface& h );
};

std::ostream&				operator<<( std::ostream& out, const HttpInterface& h );
 