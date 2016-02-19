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

#include "TcpClientConnection.h"

#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;
using namespace std;

TcpClientConnectionRef TcpClientConnection::create( asio::io_service& io )
{
	return TcpClientConnectionRef( new TcpClientConnection( io ) );
}

TcpClientConnection::TcpClientConnection( asio::io_service& io )
: mConnected( false )
{
	mClient = TcpClient::create( io );

	mClient->connectConnectEventHandler( &TcpClientEventHandler::onConnect, &mClientEventHandler );
	mClient->connectErrorEventHandler( &TcpClientEventHandler::onError, &mClientEventHandler );
	mClient->connectResolveEventHandler( &TcpClientEventHandler::onResolve, &mClientEventHandler );

	app::App::get()->getSignalUpdate().connect( bind( &TcpClientConnection::update, this ) );
}

void TcpClientConnection::close()
{
	if ( mClientEventHandler.getSession() ) {
		mClientEventHandler.getSession()->close();
	}
}

void TcpClientConnection::connect( const string& host, int16_t port )
{
	if ( isConnected() ) {
		mClientEventHandler.getSession()->close();
	}
	mClient->connect( host, port );
}

void TcpClientConnection::write( const Buffer& buffer )
{
	if ( isConnected() ) {
		mClientEventHandler.getSession()->write( buffer );
	}
}

const Buffer& TcpClientConnection::getBuffer() const
{
	return mSessionEventHandler.getBuffer();
}

bool TcpClientConnection::isConnected() const
{
	return mClientEventHandler.getSession()				&& 
		mClientEventHandler.getSession()->getSocket()	&& 
		mClientEventHandler.getSession()->getSocket()->is_open();
}

void TcpClientConnection::update()
{
	bool connected = isConnected();
	if ( mConnected != connected ) {
		mConnected = connected;
		if ( mConnected ) {
			mClientEventHandler.getSession()->connectCloseEventHandler( &TcpSessionEventHandler::onClose, &mSessionEventHandler );
			mClientEventHandler.getSession()->connectErrorEventHandler( &TcpSessionEventHandler::onError, &mSessionEventHandler );
			mClientEventHandler.getSession()->connectReadCompleteEventHandler( &TcpSessionEventHandler::onReadComplete, &mSessionEventHandler );
			mClientEventHandler.getSession()->connectReadEventHandler( &TcpSessionEventHandler::onRead, &mSessionEventHandler );
			mClientEventHandler.getSession()->connectWriteEventHandler( &TcpSessionEventHandler::onWrite, &mSessionEventHandler );
			mClientEventHandler.getSession()->read();
		}
	}
}
