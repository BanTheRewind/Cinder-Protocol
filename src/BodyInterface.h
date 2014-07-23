#pragma once

#include "ProtocolInterface.h"

class BodyInterface : public ProtocolInterface
{
public:
	//! Appends \a buffer to body.
	void				append( const ci::Buffer& buffer );
	//! Returns message body.
	const ci::Buffer&	getBody() const;
	//! Sets or replaces message body with \a body.
	void				setBody( const ci::Buffer& body );

	//! Converts entire message to ci::Buffer.
	virtual ci::Buffer		toBuffer() const;
	//! Converts entire message to std::string.
	virtual std::string	toString() const;
protected:
	BodyInterface();
	
	ci::Buffer			mBody;
};
 