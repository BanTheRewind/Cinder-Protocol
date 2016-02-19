/*
* 
* Copyright (c) 2016, Wieden+Kennedy, 
* Stephen Schieberl
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or 
* without modification, are permitted provided that the following 
* conditions are met:
* 
* Redistributions of source code must retain the above copyright 
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright 
* notice, this list of conditions and the following disclaimer in 
* the documentation and/or other materials provided with the 
* distribution.
* 
* Neither the name of the Ban the Rewind nor the names of its 
* contributors may be used to endorse or promote products 
* derived from this software without specific prior written 
* permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
*/

#pragma once

#include "FtpInterface.h"

enum : size_t
{
	FtpReplyCode_110_RESTART_MARKER_REPLY = 110,
	FtpReplyCode_120_SERVICE_READY_IN_NNN_MINUTES = 120,
	FtpReplyCode_125_DATA_CONNECTION_ALREADY_OPEN_TRANSFER_STARTING = 125,
	FtpReplyCode_150_FILE_STATUS_OKAY_ABOUT_TO_OPEN_DATA_CONNECTION = 150,
	FtpReplyCode_200_COMMAND_OKAY = 200,
	FtpReplyCode_202_COMMAND_NOT_IMPLEMENTED_SUPERFLUOUS_AT_THIS_SITE = 202,
	FtpReplyCode_211_SYSTEM_STATUS_OR_SYSTEM_HELP_REPLY = 211,
	FtpReplyCode_212_DIRECTORY_STATUS = 212,
	FtpReplyCode_213_FILE_STATUS = 213,
	FtpReplyCode_214_HELP_MESSAGE = 214,
	FtpReplyCode_215_NAME_SYSTEM_TYPE = 215,
	FtpReplyCode_220_SERVICE_READY_FOR_NEW_USER = 220,
	FtpReplyCode_221_SERVICE_CLOSING_CONTROL_CONNECTION = 221,
	FtpReplyCode_225_DATA_CONNECTION_OPEN_NO_TRANSFER_IN_PROGRESS = 225,
	FtpReplyCode_226_CLOSING_DATA_CONNECTION = 226,
	FtpReplyCode_227_ENTERING_PASSIVE_MODE = 227,
	FtpReplyCode_228_ENTERING_LONG_PASSIVE_MODE = 228,
	FtpReplyCode_229_EXTENDED_PASSIVE_MODE_ENTERED = 229,
	FtpReplyCode_230_USER_LOGGED_IN_PROCEED = 230,
	FtpReplyCode_250_REQUESTED_FILE_ACTION_OKAY_COMPLETED = 250,
	FtpReplyCode_257_PATHNAME_CREATED = 257,
	FtpReplyCode_331_USER_NAME_OKAY_NEED_PASSWORD = 331,
	FtpReplyCode_332_NEED_ACCOUNT_FOR_LOGIN = 332,
	FtpReplyCode_350_REQUESTED_FILE_ACTION_PENDING_FURTHER_INFORMATION = 350,
	FtpReplyCode_421_SERVICE_NOT_AVAILABLE_CLOSING_CONTROL_CONNECTION = 421,
	FtpReplyCode_425_CANNOT_OPEN_DATA_CONNECTION = 425,
	FtpReplyCode_426_CONNECTION_CLOSED_TRANSFER_ABORTED = 426,
	FtpReplyCode_450_REQUESTED_FILE_ACTION_NOT_TAKEN = 450,
	FtpReplyCode_451_REQUESTED_ACTION_ABORTED_LOCAL_ERROR_IN_PROCESSING = 451,
	FtpReplyCode_452_REQUESTED_ACTION_NOT_TAKEN = 452,
	FtpReplyCode_500_SYNTAX_ERROR_COMMAND_UNRECOGNIZED = 500,
	FtpReplyCode_501_SYNTAX_ERROR_IN_PARAMETERS_OR_ARGUMENTS = 501,
	FtpReplyCode_502_COMMAND_NOT_IMPLEMENTED = 502,
	FtpReplyCode_503_BAD_SEQUENCE_OF_COMMANDS = 503,
	FtpReplyCode_504_COMMAND_NOT_IMPLEMENTED_FOR_THAT_PARAMETER = 504,
	FtpReplyCode_521_SUPPORTED_ADDRESS_FAMILIES_ARE = 521,
	FtpReplyCode_522_PROTOCOL_NOT_SUPPORTED = 522,
	FtpReplyCode_530_NOT_LOGGED_IN = 530,
	FtpReplyCode_532_NEED_ACCOUNT_FOR_STORING_FILES = 532,
	FtpReplyCode_550_REQUESTED_ACTION_NOT_TAKEN = 550,
	FtpReplyCode_551_REQUESTED_ACTION_ABORTED_PAGE_TYPE_UNKNOWN = 551,
	FtpReplyCode_552_REQUESTED_FILE_ACTION_ABORTED = 552,
	FtpReplyCode_553_REQUESTED_ACTION_NOT_TAKEN = 553,
	FtpReplyCode_554_REQUESTED_ACTION_NOT_TAKEN_INVALID_REST_PARAMETER = 554,
	FtpReplyCode_555_REQUESTED_ACTION_NOT_TAKEN_TYPE_OR_STRU_MISMATCH = 555
} typedef FtpReplyCode;

class FtpResponse : public FtpInterface
{
public:
	//! Creates a FTP response with reply code \a replyCode and value \a value.
	explicit FtpResponse( FtpReplyCode replyCode = FtpReplyCode_200_COMMAND_OKAY, 
						  const std::string& value = "" );
	//! Creates a FTP response with reply code \a replyCode and value \a value.
	explicit FtpResponse( size_t replyCode, const std::string& value = "" );

	//! Returns string representing description of FtpReplyCode \a c.
	static std::string	getReplyCodeDescription( FtpReplyCode c );

	//! Returns reply code as size_t.
	size_t				getReplyCode() const;
	//! Sets reply code to \a c.
	void				setReplyCode( FtpReplyCode c );
	//! Sets reply code to \a c.
	void				setReplyCode( size_t c );

	//! Exception representing missing reply code.
	class ExcReplyCodeNotFound : public ci::Exception
	{
	public:
		ExcReplyCodeNotFound( size_t replyCode ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
};
