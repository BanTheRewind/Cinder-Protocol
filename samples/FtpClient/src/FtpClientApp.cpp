#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"

#include "boost/algorithm/string.hpp"

#include "FtpRequest.h"
#include "FtpResponse.h"

#include "TcpClientConnection.h"

class FtpClientApp : public ci::app::AppBasic 
{
public:
	void						draw();
	void						setup();
	void						update();
private:
	TcpClientConnectionRef		mConnectionControl;
	std::string					mHost;
	
	FtpRequest					mFtpRequest;
	FtpResponse					mFtpResponse;
	
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

void FtpClientApp::setup()
{
	gl::enable( GL_TEXTURE_2D );
	
	mFont			= Font( "Georgia", 24 );
	mFrameRate		= 0.0f;
	mFullScreen		= false;
	
	mParams = params::InterfaceGl::create( "Params", Vec2i( 200, 120 ) );
	mParams->addParam( "Frame rate",	&mFrameRate,			"", true );
	mParams->addParam( "Full screen",	&mFullScreen,			"key=f" );
	mParams->addButton( "Quit",			[ & ]() { quit(); },	"key=q" );
	
	mConnectionControl = TcpClientConnection::create( io_service() );

	mConnectionControl->connect( "www.bantherewind.com", 21 );
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

	if ( mConnectionControl->isConnected()	&& 
		 mConnectionControl->getBuffer()	&& 
		 mConnectionControl->getBuffer().getDataSize() > 0 ) {
		const Buffer& buffer = mConnectionControl->getBuffer();

		string s = SessionInterface::bufferToString( buffer );

		mFtpResponse.parse( buffer );
		mText.push_back( mFtpResponse.toString() );

		switch ( mFtpResponse.getReplyCode() ) {
		case FtpReplyCode_220_SERVICE_READY_FOR_NEW_USER:
			mFtpRequest.set( FtpCommand_USER, "****" );
			mText.push_back( mFtpRequest.toString() );
			// TODO get toBuffer to work here
			mConnectionControl->write( FtpRequest::stringToBuffer( mFtpRequest.toString() ) );
			break;
		case FtpReplyCode_230_USER_LOGGED_IN_PROCEED:
			// TODO open data connection here
			break;
		case FtpReplyCode_331_USER_NAME_OKAY_NEED_PASSWORD:
			mFtpRequest.set( FtpCommand_PASS, "****" );
			// TODO get toBuffer to work here
			mText.push_back( mFtpRequest.toString() );
			mConnectionControl->write( FtpRequest::stringToBuffer( mFtpRequest.toString() ) );
			break;
		default:
			mConnectionControl->close();
			break;
		}
	}
}

CINDER_APP_BASIC( FtpClientApp, RendererGl )
