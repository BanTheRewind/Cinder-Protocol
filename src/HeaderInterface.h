#pragma once

#include "ProtocolInterface.h"

typedef std::map<std::string, std::string> HeaderMap;

class HeaderInterface : public ProtocolInterface
{
public:
	//! Parses \a headerMap into a header map object.
	static HeaderMap	stringToHeaderMap( const std::string& headerMap );
	//! Return string representation of \a headerMap.
	static std::string	headerMapToString( const HeaderMap& headerMap );
	//! Returns copy of buffer with header removed.
	static ci::Buffer	removeHeader( const ci::Buffer& buffer );

	//! Erases header named \a field.
	void				eraseHeader( const std::string& field );
	//! Returns header value for \a field.
	const std::string&	getHeader( const std::string& field );
	//! Return header map.
	HeaderMap&			getHeaders();
	//! Return header map.
	const HeaderMap&	getHeaders() const;
	//! Returns true if a valid header exists.
	bool				hasHeader() const;
	//! Set header \a value for \a field. Overwrites existing value with same key.
	void				setHeader( const std::string& field, const std::string& value );
	//! Set header field from \a kvp. Overwrites existing value with same key.
	void				setHeader( const KeyValuePair& kvp );
	// Sets all header fields from \a headerMap. Overwrites existing values.
	void				setHeaders( const HeaderMap& headerMap );
	//! Parses \a buffer into headers.
	virtual void		parse( const ci::Buffer& buffer );
	virtual void		parseHeader( const std::string& header ) = 0;

	//! Converts entire message to ci::Buffer.
	virtual ci::Buffer	toBuffer() const;
	//! Converts entire message to std::string.
	virtual std::string	toString() const;
	
	//! Exception representing missing header
	class ExcHeaderNotFound : public ci::Exception
	{
	public:
		ExcHeaderNotFound( const std::string& field ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	HeaderInterface();
	
	virtual std::string	headerToString() const = 0;

	bool				mHasHeader;
	HeaderMap			mHeaderMap;
};

  