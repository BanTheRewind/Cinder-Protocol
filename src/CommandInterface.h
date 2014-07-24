#pragma once

#include "ProtocolInterface.h"

class CommandInterface : public ProtocolInterface
{
public:
	//! Converts entire message to ci::Buffer.
	virtual ci::Buffer	toBuffer() const;
	//! Converts entire message to std::string.
	virtual std::string	toString() const;
	
	//! Exception representing missing command
	class ExcCommandNotFound : public ci::Exception
	{
	public:
		ExcCommandNotFound( const std::string& command ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	CommandInterface();

	std::vector<KeyValuePair>	mCommands;
};
 