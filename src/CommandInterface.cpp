#include "CommandInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

CommandInterface::CommandInterface()
: ProtocolInterface(), mHasCommand( false )
{
}

CommandMap CommandInterface::stringToCommandMap( const string& c )
{
	vector<string> tokens;
	boost::split( tokens, c, boost::is_any_of( sCrLf ) );
	CommandMap commandMap;
	for ( vector<string>::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter ) {
		if ( !iter->empty() ) {
			KeyValuePair kvp = stringToKeyValuePair( *iter );
			commandMap.insert( kvp );
		}
	}
	return commandMap;
}

string CommandInterface::commandMapToString( const CommandMap& c )
{
	string commandMap = "";
	for ( CommandMap::const_iterator iter = c.begin(); iter != c.end(); ++iter ) {
		commandMap += keyValuePairToString( *iter ) + sCrLf;
	}
	return commandMap;
}

KeyValuePair CommandInterface::stringToKeyValuePair( const string& kvp )
{
	vector<string> tokens;
	boost::split( tokens, kvp, boost::is_any_of( ":" ) );
	if ( tokens.size() < 2 ) {
		throw ExcKeyValuePairInvalid( kvp );
	} else {
		string key		= boost::trim_copy( tokens.at( 0 ) );
		tokens.erase( tokens.begin() );
		string value	= boost::join( tokens, ":" );
		value			= boost::trim_copy( value );
	
		KeyValuePair kvp( key, value );
		return kvp;
	}
}

string CommandInterface::keyValuePairToString( const KeyValuePair& kvp )
{
	return kvp.first + ": " + kvp.second;
}

Buffer CommandInterface::removeCommand( const Buffer& buffer )
{
	string msg		= bufferToString( buffer );
	size_t offset	= msg.find( sCrLf + sCrLf );
	size_t sz		= buffer.getDataSize();
	if ( offset < sz ) {
		size_t len = ( sz - offset ) - 4;
		Buffer body( len );
		char_traits<char>::copy( (char*)body.getData(), (char*)buffer.getData() + ( offset + 4 ), len );
		return body;
	} else {
		return Buffer();
	}
}

void CommandInterface::eraseCommand( const string& field )
{
	if ( mCommandMap.find( field ) == mCommandMap.end() ) {
		throw ExcCommandNotFound( field );
	} else {
		mCommandMap.erase( field );
	}
}

const string& CommandInterface::getCommand( const string& field )
{
	if ( mCommandMap.find( field ) == mCommandMap.end() ) {
		throw ExcCommandNotFound( field );
	} else {
		return mCommandMap[ field ];
	}
}

CommandMap& CommandInterface::getCommands()
{
	return mCommandMap;
}

const CommandMap& CommandInterface::getCommands() const
{
	return mCommandMap;
}

bool CommandInterface::hasCommand() const
{
	return mHasCommand;
}

void CommandInterface::setCommand( const string& field, const string& value )
{
	mCommandMap[ field ] = value;
}

void CommandInterface::setCommand( const KeyValuePair& kvp )
{
	setCommand( kvp.first, kvp.second );
}

void CommandInterface::setCommands( const CommandMap& commandMap )
{
	mCommandMap = commandMap;
}

void CommandInterface::parse( const Buffer& buffer )
{
	string msg		= bufferToString( buffer );
	parseCommand( msg );
}

Buffer CommandInterface::toBuffer() const
{
	string command	= commandToString();
	size_t sz		= command.size();
	
	Buffer buffer( sz );
	char_traits<char>::copy( (char*)buffer.getData(), (char*)&command[ 0 ], sz );
	
	return buffer;
}

string CommandInterface::toString() const
{
	return commandToString();
}

CommandInterface::ExcCommandNotFound::ExcCommandNotFound( const string& field ) throw()
{
	sprintf( mMessage, "Command field \"%s\" not found", field.c_str() );
}

CommandInterface::ExcKeyValuePairInvalid::ExcKeyValuePairInvalid( const string& kvp ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid key:value pair", kvp.c_str() );
}

ostream& operator<<( ostream& out, const CommandInterface& p )
{
	out << p.toString();
	return out;
}
