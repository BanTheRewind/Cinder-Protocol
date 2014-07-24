#include"FtpInterface.h"

#include"cinder/Utilities.h"

using namespace ci;
using namespace std;

FtpInterface::FtpInterface()
: BodyInterface(), CommandInterface()
{
}
 
string FtpInterface::commandToString( FtpCommand c )
{
	static vector<string> commands;
	if ( commands.empty() ) {
		commands.push_back( "ABOR" );
		commands.push_back( "ACCT" );
		commands.push_back( "ADAT" );
		commands.push_back( "ALLO" );
		commands.push_back( "APPE" );
		commands.push_back( "AUTH" );
		commands.push_back( "CCC" );
		commands.push_back( "CDUP" );
		commands.push_back( "CONF" );
		commands.push_back( "CWD" );
		commands.push_back( "DELE" );
		commands.push_back( "ENC" );
		commands.push_back( "EPRT" );
		commands.push_back( "EPSV" );
		commands.push_back( "FEAT" );
		commands.push_back( "HELP" );
		commands.push_back( "LANG" );
		commands.push_back( "LIST" );
		commands.push_back( "LPRT" );
		commands.push_back( "LPSV" );
		commands.push_back( "MDTM" );
		commands.push_back( "MIC" );
		commands.push_back( "MKD" );
		commands.push_back( "MLSD" );
		commands.push_back( "MLST" );
		commands.push_back( "MODE" );
		commands.push_back( "NLST" );
		commands.push_back( "NOOP" );
		commands.push_back( "OPTS" );
		commands.push_back( "PASS" );
		commands.push_back( "PASV" );
		commands.push_back( "PBSZ" );
		commands.push_back( "PORT" );
		commands.push_back( "PROT" );
		commands.push_back( "PWD" );
		commands.push_back( "QUIT" );
		commands.push_back( "REIN" );
		commands.push_back( "REST" );
		commands.push_back( "RETR" );
		commands.push_back( "RMD" );
		commands.push_back( "RNFR" );
		commands.push_back( "RNTO" );
		commands.push_back( "SITE" );
		commands.push_back( "SIZE" );
		commands.push_back( "SMNT" );
		commands.push_back( "STAT" );
		commands.push_back( "STOR" );
		commands.push_back( "STOU" );
		commands.push_back( "STRU" );
		commands.push_back( "SYST" );
		commands.push_back( "TYPE" );
		commands.push_back( "USER" );
		commands.push_back( "XCUP" );
		commands.push_back( "XMKD" );
		commands.push_back( "XPWD" );
		commands.push_back( "XRCP" );
		commands.push_back( "XRMD" );
		commands.push_back( "XRSQ" );
		commands.push_back( "XSEM" );
		commands.push_back( "XSEN" );
	}
	if ( (size_t)c > commands.size() ) {
		throw ExcCommandNotFound( "FTP command ID" + ci::toString( c ) +" is invalid" );
	}
	return commands.at( c );
}

string FtpInterface::commandToDescriptionString( FtpCommand c )
{
	static vector<string> commands;
	if ( commands.empty() ) {
		commands.push_back( "Abort" );
		commands.push_back( "Account" );
		commands.push_back( "Authentication data" );
		commands.push_back( "Allocate" );
		commands.push_back( "Append" );
		commands.push_back( "Authentication" );
		commands.push_back( "Clear command channel" );
		commands.push_back( "Change to parent directory" );
		commands.push_back( "Confidentiality protected command" );
		commands.push_back( "Change working directory" );
		commands.push_back( "Delete" );
		commands.push_back( "Privacy protected command" );
		commands.push_back( "Extended data port" );
		commands.push_back( "Extended passive" );
		commands.push_back( "Feature" );
		commands.push_back( "Help" );
		commands.push_back( "Language negotiation" );
		commands.push_back( "List" );
		commands.push_back( "Long data port" );
		commands.push_back( "Long passive" );
		commands.push_back( "File modification time" );
		commands.push_back( "Integrity protected command" );
		commands.push_back( "Make directory" );
		commands.push_back( "List" );
		commands.push_back( "List" );
		commands.push_back( "Transfer mode" );
		commands.push_back( "Name list" );
		commands.push_back( "No operation" );
		commands.push_back( "Options" );
		commands.push_back( "Password" );
		commands.push_back( "Passive mode" );
		commands.push_back( "Protection buffer size" );
		commands.push_back( "Data port" );
		commands.push_back( "Data channel protection level" );
		commands.push_back( "Print working directory" );
		commands.push_back( "Logout" );
		commands.push_back( "Reinitialize" );
		commands.push_back( "Restart of interrupted transfer" );
		commands.push_back( "Retrieve" );
		commands.push_back( "Remove directory" );
		commands.push_back( "Rename from" );
		commands.push_back( "Rename to" );
		commands.push_back( "Site parameters" );
		commands.push_back( "File size" );
		commands.push_back( "Structure mount" );
		commands.push_back( "Status" );
		commands.push_back( "Store" );
		commands.push_back( "Store unique" );
		commands.push_back( "File structure" );
		commands.push_back( "System" );
		commands.push_back( "Representation type" );
		commands.push_back( "User name" );
		commands.push_back( "Change to the parent of the current working directory" );
		commands.push_back( "Make a directory" );
		commands.push_back( "Print the current working directory" );
		commands.push_back( "Specify recipients after scheme selection" );
		commands.push_back( "Remove the directory" );
		commands.push_back( "Scheme selection" );
		commands.push_back( "Send, mail if cannot" );
		commands.push_back( "Send to terminal" );
	}
	if ( (size_t)c > commands.size() ) {
		throw ExcCommandNotFound( ci::toString( c ) );
	}
	return commands.at( c );
}

string FtpInterface::replyCodeToString( FtpReplyCode c )
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

Buffer FtpInterface::toBuffer() const
{
	Buffer buffer;
	return buffer;
}

string FtpInterface::toString() const
{
	return "";
}

FtpInterface::ExcReplyCodeNotFound::ExcReplyCodeNotFound( size_t replyCode ) throw()
{
	sprintf( mMessage, "Reply code \"%u\" not found", replyCode );
}

ostream& operator<<( ostream& out, const FtpInterface& p )
{
	out << p.toString();
	return out;
}
 