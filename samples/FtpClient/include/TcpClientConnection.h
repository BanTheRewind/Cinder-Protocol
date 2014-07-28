#pragma once

#include "TcpClient.h"
#include "TcpClientEventHandler.h"
#include "TcpSessionEventHandler.h"

typedef std::shared_ptr<class TcpClientConnection> TcpClientConnectionRef;

class TcpClientConnection
{
public:
	static TcpClientConnectionRef	create( boost::asio::io_service& io );

	void					close();
	void					connect( const std::string& host, int16_t port );
	void					write( const ci::Buffer& buffer );

	const ci::Buffer&		getBuffer() const;
	bool					isConnected() const;
protected:
	TcpClientConnection( boost::asio::io_service& io );

	void					update();

	bool					mConnected;

	TcpClientRef			mClient;
	TcpClientEventHandler	mClientEventHandler;
	TcpSessionEventHandler	mSessionEventHandler;
};