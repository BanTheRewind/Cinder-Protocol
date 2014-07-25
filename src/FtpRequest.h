#pragma once

#include "KeyValuePairInterface.h"

enum : size_t
{
	FtpCommand_ABOR,
	FtpCommand_ACCT,
	FtpCommand_ADAT,
	FtpCommand_ALLO,
	FtpCommand_APPE,
	FtpCommand_AUTH,
	FtpCommand_CCC,
	FtpCommand_CDUP,
	FtpCommand_CONF,
	FtpCommand_CWD,
	FtpCommand_DELE,
	FtpCommand_ENC,
	FtpCommand_EPRT,
	FtpCommand_EPSV,
	FtpCommand_FEAT,
	FtpCommand_HELP,
	FtpCommand_LANG,
	FtpCommand_LIST,
	FtpCommand_LPRT,
	FtpCommand_LPSV,
	FtpCommand_MDTM,
	FtpCommand_MIC,
	FtpCommand_MKD,
	FtpCommand_MLSD,
	FtpCommand_MLST,
	FtpCommand_MODE,
	FtpCommand_NLST,
	FtpCommand_NOOP,
	FtpCommand_OPTS,
	FtpCommand_PASS,
	FtpCommand_PASV,
	FtpCommand_PBSZ,
	FtpCommand_PORT,
	FtpCommand_PROT,
	FtpCommand_PWD,
	FtpCommand_QUIT,
	FtpCommand_REIN,
	FtpCommand_REST,
	FtpCommand_RETR,
	FtpCommand_RMD,
	FtpCommand_RNFR,
	FtpCommand_RNTO,
	FtpCommand_SITE,
	FtpCommand_SIZE,
	FtpCommand_SMNT,
	FtpCommand_STAT,
	FtpCommand_STOR,
	FtpCommand_STOU,
	FtpCommand_STRU,
	FtpCommand_SYST,
	FtpCommand_TYPE,
	FtpCommand_USER,
	FtpCommand_XCUP,
	FtpCommand_XMKD,
	FtpCommand_XPWD,
	FtpCommand_XRCP,
	FtpCommand_XRMD,
	FtpCommand_XRSQ,
	FtpCommand_XSEM,
	FtpCommand_XSEN
} typedef FtpCommand;

class FtpRequest : public KeyValuePairInterface
{
public:
	//! Creates a FTP request with command \a command and value \a value.
	FtpRequest( const std::string& command = "", const std::string& value = "" );

	//! Get string representing command \c.
	static std::string	getCommandString( FtpCommand c );
	//! Get string representing description of command \c.
	static std::string	getCommandDescription( FtpCommand c );

	//! Returns FTP command representing command.
	FtpCommand			getCommand() const;
	//! Returns string representing command.
	const std::string&	getCommandString() const;
	//! Sets command to string \a c.
	void				setCommand( const std::string& c );
	//! Sets command to FtpCommand \a c.
	void				setCommand( FtpCommand c );
	//! Sets command to string \a c and value to \a v.
	void				set( const std::string& c, const std::string& v );
	//! Sets command to FtpCommand \a c and value to \a v.
	void				set( FtpCommand c, const std::string& v );

	//! Exception representing missing FTP command.
	class ExcCommandNotFound : public ci::Exception
	{
	public:
		ExcCommandNotFound( const std::string& msg ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
};
