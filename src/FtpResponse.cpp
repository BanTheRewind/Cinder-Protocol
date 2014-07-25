#include "FtpResponse.h"

#include"cinder/Utilities.h"

using namespace ci;
using namespace std;

FtpResponse::FtpResponse( FtpResponse::FtpReplyCode replyCode, const string& value )
: KeyValuePairInterface( ci::toString( (size_t)replyCode ), value )
{
}

FtpResponse::FtpResponse( size_t replyCode, const string& value )
: KeyValuePairInterface( ci::toString( replyCode ), value )
{
}


string FtpResponse::getReplyCodeDescription( FtpResponse::FtpReplyCode c )
{
	static vector<string> replyCodes;
	if ( replyCodes.empty() ) {
		replyCodes.push_back( "Restart marker reply" );
		replyCodes.push_back( "Service ready in nnn minutes" );
		replyCodes.push_back( "Data connection already open; transfer starting" );
		replyCodes.push_back( "File status okay; about to open data connection" );
		replyCodes.push_back( "Command okay" );
		replyCodes.push_back( "Command not implemented, superfluous at this site" );
		replyCodes.push_back( "System status, or system help reply" );
		replyCodes.push_back( "Directory status" );
		replyCodes.push_back( "File status" );
		replyCodes.push_back( "Help message" );
		replyCodes.push_back( "NAME system type" );
		replyCodes.push_back( "Service ready for new user" );
		replyCodes.push_back( "Service closing control connection" );
		replyCodes.push_back( "Data connection open; no transfer in progress" );
		replyCodes.push_back( "Closing data connection" );
		replyCodes.push_back( "Entering passive mode..." );
		replyCodes.push_back( "Entering long passive mode" );
		replyCodes.push_back( "Extended passive mode entered" );
		replyCodes.push_back( "User logged in, proceed" );
		replyCodes.push_back( "Requested file action okay, completed" );
		replyCodes.push_back( "Pathname created" );
		replyCodes.push_back( "User name okay, need password" );
		replyCodes.push_back( "Need account for login" );
		replyCodes.push_back( "Requested file action pending further information" );
		replyCodes.push_back( "Service not available, closing control connection" );
		replyCodes.push_back( "Cannot open data connection" );
		replyCodes.push_back( "Connection closed; transfer aborted" );
		replyCodes.push_back( "Requested file action not taken" );
		replyCodes.push_back( "Requested action aborted: local error in processing" );
		replyCodes.push_back( "Requested action not taken" );
		replyCodes.push_back( "Syntax error, command unrecognised" );
		replyCodes.push_back( "Syntax error in parameters or arguments" );
		replyCodes.push_back( "Command not implemented" );
		replyCodes.push_back( "Bad sequence of commands" );
		replyCodes.push_back( "Command not implemented for that parameter" );
		replyCodes.push_back( "Supported address families are..." );
		replyCodes.push_back( "Protocol not supported" );
		replyCodes.push_back( "Not logged in" );
		replyCodes.push_back( "Need account for storing files" );
		replyCodes.push_back( "Requested action not taken" );
		replyCodes.push_back( "Requested action aborted: page type unknown" );
		replyCodes.push_back( "Requested file action aborted" );
		replyCodes.push_back( "Requested action not taken" );
		replyCodes.push_back( "Requested action not taken: invalid REST parameter" );
		replyCodes.push_back( "Requested action not taken: TYPE or STRU mismatch" );
	}
	if ( (size_t)c > replyCodes.size() ) {
		throw ExcReplyCodeNotFound( (size_t)c );
	}
	return replyCodes.at( c );
}

size_t FtpResponse::getReplyCode() const
{
	return fromString<size_t>( mKeyValuePair.first );
}

void FtpResponse::setReplyCode( FtpResponse::FtpReplyCode c )
{
	mKeyValuePair.first = ci::toString( (size_t)c );
}

void FtpResponse::setReplyCode( size_t c )
{
	mKeyValuePair.first = ci::toString( c );
}

FtpResponse::ExcReplyCodeNotFound::ExcReplyCodeNotFound( size_t replyCode ) throw()
{
	sprintf( mMessage, "Reply code \"%u\" not found", replyCode );
}
 