#pragma once

#include "ProtocolInterface.h"

typedef std::map<std::string, std::string> CommandMap;

class CommandInterface : public ProtocolInterface
{
public:
	//! Parses \a kvp into a key-value pair object.
	static KeyValuePair	stringToKeyValuePair( const std::string& kvp );
	//! Returns string representation of key-value pair \a kvp.
	static std::string	keyValuePairToString( const KeyValuePair& kvp );

	//! Parses \a commandMap into a command map object.
	static CommandMap	stringToCommandMap( const std::string& commandMap );
	//! Return string representation of \a commandMap.
	static std::string	commandMapToString( const CommandMap& commandMap );
	//! Returns copy of buffer with command removed.
	static ci::Buffer	removeCommand( const ci::Buffer& buffer );

	//! Erases command named \a command.
	void				eraseCommand( const std::string& command );
	//! Returns command value for \a command.
	const std::string&	getCommand( const std::string& command );
	//! Return command map.
	CommandMap&			getCommands();
	//! Return command map.
	const CommandMap&	getCommands() const;
	//! Returns true if a valid command exists.
	bool				hasCommand() const;
	//! Set \a value for \a command. Overwrites existing value with same key.
	void				setCommand( const std::string& command, const std::string& value );
	//! Set command from \a kvp. Overwrites existing value with same key.
	void				setCommand( const KeyValuePair& kvp );
	// Sets all commands from \a commandMap. Overwrites existing values.
	void				setCommands( const CommandMap& commandMap );
	//! Parses \a buffer into commands.
	virtual void		parse( const ci::Buffer& buffer );
	virtual void		parseCommand( const std::string& command ) = 0;

	//! Converts entire message to ci::Buffer.
	virtual ci::Buffer	toBuffer() const;
	//! Converts entire message to std::string.
	virtual std::string	toString() const;
	
	//! Exception representing missing command
	class ExcCommandNotFound : public ci::Exception
	{
	public:
		ExcCommandNotFound( const std::string &command ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
protected:
	CommandInterface();
	
	static std::string	sCrLf;
	static std::string	sLf;

	virtual std::string	commandToString() const = 0;

	bool				mHasCommand;
	CommandMap			mCommandMap;
};
 