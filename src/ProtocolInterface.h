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
	//! Return string \a value as Buffer.
	static ci::Buffer		stringToBuffer( const std::string& value );
	//! Returns string representation of \a buffer.
	static std::string		bufferToString( const ci::Buffer& buffer );
	
	//! Parses \a headerMap into a header map object.
	static HeaderMap		stringToHeaderMap( const std::string& headerMap );
	//! Return string representation of \a headerMap.
	static std::string		headerMapToString( const HeaderMap& headerMap );
	
	//! Parses \a kvp into a key-value pair object.
	static KeyValuePair		stringToKeyValuePair( const std::string& kvp );
	//! Returns string representation of key-value pair \a kvp.
	static std::string		keyValuePairToString( const KeyValuePair& kvp );

	//! Returns copy of buffer with header removed.
	static ci::Buffer		removeHeader( const ci::Buffer& buffer );
	
	//! Appends \a buffer to body.
	void					append( const ci::Buffer& buffer );
	//! Returns message body.
	const ci::Buffer&		getBody() const;
	//! Sets or replaces message body with \a body.
	void					setBody( const ci::Buffer& body );
	
	//! Erases header named \a field.
	void					eraseHeader( const std::string& field );
	
	//! Returns header value for \a field.
	const std::string&		getHeader( const std::string& field );
	//! Return header map.
	HeaderMap&				getHeaders();
	//! Return header map.
	const HeaderMap&		getHeaders() const;
	//! Returns true if a valid header exists.
	bool					hasHeader() const;
	
	//! Set header \a value for \a field. Overwrites existing value with same key.
	void					setHeader( const std::string& field, const std::string& value );
	//! Set header field from \a kvp. Overwrites existing value with same key.
	void					setHeader( const KeyValuePair& kvp );
	// Sets all header fields from \a headerMap. Overwrites existing values.
	void					setHeaders( const HeaderMap& headerMap );
	
	/*! Parses header and body from \a msg. Throws exceptions for incomplete 
		or invalid headers. */
	virtual void			parse( const std::string& msg );
	/*! Parses header and body from \a buffer. Throws exceptions for incomplete 
		or invalid headers. */
	void					parse( const ci::Buffer& buffer );
	virtual void			parseHeader( const std::string& header ) = 0;

	//! Converts entire message to ci::Buffer.
	ci::Buffer				toBuffer() const;
	//! Converts entire message to std::string.
	std::string				toString() const;
	
	//! Exception representing missing header
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
	
	//! Exception representing invalid key-value pait
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
	ProtocolInterface();
	
	static std::string		sCrLf;
	static std::string		sLf;
	
	virtual std::string		headerToString() const = 0;

	ci::Buffer				mBody;
	bool					mHasHeader;
	HeaderMap				mHeaderMap;
	
	friend std::ostream&	operator<<( std::ostream& out, const ProtocolInterface& h );
};

std::ostream&				operator<<( std::ostream& out, const ProtocolInterface& h );
