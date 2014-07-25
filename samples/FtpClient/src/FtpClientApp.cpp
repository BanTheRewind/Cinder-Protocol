#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"

#include "boost/algorithm/string.hpp"

#include "FtpRequest.h"
#include "FtpResponse.h"
#include "TcpClient.h"

class FtpClientApp : public ci::app::AppBasic 
{
public:
	void						draw();
	void						setup();
	void						update();
private:
	TcpClientRef				mClient;
	TcpSessionRef				mSession;
	std::string					mHost;
	
	FtpRequest					mFtpRequest;
	FtpResponse					mFtpResponse;
	
	void						write();
	
	void						onClose();
	void						onConnect( TcpSessionRef session );
	void						onError( std::string err, size_t bytesTransferred );
	void						onRead( ci::Buffer buffer );
	void						onResolve();
	void						onWrite( size_t bytesTransferred );
	
	ci::Font					mFont;
	std::vector<std::string>	mText;
	ci::gl::TextureRef			mTexture;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;
};

#include "cinder/Utilities.h"
#include <fstream>
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

void FtpClientApp::draw()
{
	gl::clear( Colorf::black() );
	gl::setMatricesWindow( getWindowSize() );
	
	if ( mTexture ) {
		gl::draw( mTexture, Vec2i( 250, 20 ) );
	}
	
	mParams->draw();
}

void FtpClientApp::onClose()
{
	mText.push_back( "Disconnected" );
}

void FtpClientApp::onConnect( TcpSessionRef session )
{
	mSession		= session;
	mText.push_back( "Connected" );
	
	mSession->connectCloseEventHandler( &FtpClientApp::onClose, this );
	mSession->connectErrorEventHandler( &FtpClientApp::onError, this );
	mSession->connectReadEventHandler( &FtpClientApp::onRead, this );
	mSession->connectWriteEventHandler( &FtpClientApp::onWrite, this );
	
	mSession->write( mFtpRequest.toBuffer() );
}

void FtpClientApp::onError( string err, size_t bytesTransferred )
{
	string text = "Error";
	if ( !err.empty() ) {
		text += ": " + err;
	}
	mText.push_back( text );
}

void FtpClientApp::onRead( ci::Buffer buffer )
{
	mSession->close();
}

void FtpClientApp::onResolve()
{
	mText.push_back( "Endpoint resolved" );
}

void FtpClientApp::onWrite( size_t bytesTransferred )
{
	mText.push_back(toString( bytesTransferred ) + " bytes written" );
	mSession->read();
}

void FtpClientApp::setup()
{
	gl::enable( GL_TEXTURE_2D );
	
	mFont			= Font( "Georgia", 24 );
	mFrameRate		= 0.0f;
	mFullScreen		= false;
	mHost			= "127.0.0.1";
	
	mParams = params::InterfaceGl::create( "Params", Vec2i( 200, 150 ) );
	mParams->addParam( "Frame rate",	&mFrameRate,			"", true );
	mParams->addParam( "Full screen",	&mFullScreen,			"key=f" );
	mParams->addParam( "Host",			&mHost );
	mParams->addButton( "Write",		[ & ]() { write(); },	"key=w" );
	mParams->addButton( "Quit",			[ & ]() { quit(); },	"key=q" );
	
	mClient = TcpClient::create( io_service() );
	mClient->connectConnectEventHandler( &FtpClientApp::onConnect, this );
	mClient->connectErrorEventHandler( &FtpClientApp::onError, this );
	mClient->connectResolveEventHandler( &FtpClientApp::onResolve, this );
}

void FtpClientApp::update()
{
	mFrameRate = getFrameRate();
	
	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
		mFullScreen = isFullScreen();
	}

	if ( !mText.empty() ) {
		TextBox tbox = TextBox()
			.alignment( TextBox::LEFT )
			.font( mFont )
			.size( Vec2i( getWindowWidth() - 250, TextBox::GROW ) )
			.text( "" );
		for ( vector<string>::const_reverse_iterator iter = mText.rbegin(); iter != mText.rend(); ++iter ) {
			tbox.appendText( "> " + *iter + "\n" );
		}
		tbox.setColor( ColorAf( 1.0f, 0.8f, 0.75f, 1.0f ) );
		tbox.setBackgroundColor( ColorAf::black() );
		tbox.setPremultiplied( false );
		mTexture = gl::Texture::create( tbox.render() );
		while ( mText.size() > 75 ) {
			mText.erase( mText.begin() );
		}
	}
}

void FtpClientApp::write()
{
	if ( mSession && mSession->getSocket()->is_open() ) {
		return;
	}
	
	mText.push_back( "Connecting to:\n" + mHost + ":21" );
	
	mClient->connect( mHost, 21 );
}

CINDER_APP_BASIC( FtpClientApp, RendererGl )
