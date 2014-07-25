#pragma once

#include "ProtocolInterface.h"

class KeyValuePairInterface : public ProtocolInterface
{
public:
	//! Returns string representing value.
	const std::string&		getValue() const;
	//! Sets value to \a v.
	void					setValue( const std::string& v );

	//! Parses \a buffer to key-value pair.
	void					parse( const ci::Buffer& buffer );

	//! Converts message to ci::Buffer.
	ci::Buffer				toBuffer() const;
	//! Converts message to string.
	virtual std::string		toString() const;
protected:
	KeyValuePairInterface( const std::string& key = "", const std::string& value = "" );

	KeyValuePair			mKeyValuePair;

	friend std::ostream&	operator<<( std::ostream& out, const KeyValuePairInterface& h );
};

std::ostream&				operator<<( std::ostream& out, const KeyValuePairInterface& h );
