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

#include "CinderAsio.h"

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"

#include "boost/algorithm/string.hpp"

#include "FtpRequest.h"
#include "FtpResponse.h"

#include "TcpClientConnection.h"

class FtpClientApp : public ci::app::App
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

#include "cinder/app/RendererGl.h"
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
		gl::draw( mTexture, ivec2( 250, 20 ) );
	}
	
	mParams->draw();
}

void FtpClientApp::setup()
{
	gl::enable( GL_TEXTURE_2D );
	
	mFont			= Font( "Georgia", 24 );
	mFrameRate		= 0.0f;
	mFullScreen		= false;
	
	mParams = params::InterfaceGl::create( "Params", ivec2( 200, 120 ) );
	mParams->addParam( "Frame rate",	&mFrameRate,			"", true );
	mParams->addParam( "Full screen",	&mFullScreen ).key( "f" );
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
			.size( ivec2( getWindowWidth() - 250, TextBox::GROW ) )
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

		string s = ProtocolInterface::bufferToString( buffer );
		
		mFtpResponse.parse( buffer );
		mText.push_back( mFtpResponse.toString() );

		switch ( mFtpResponse.getReplyCode() ) {
		case FtpReplyCode_220_SERVICE_READY_FOR_NEW_USER:
			mFtpRequest.set( FtpCommand_USER, "****" );
			mText.push_back( mFtpRequest.toString() );
			// TODO get toBuffer to work here
			mConnectionControl->write( mFtpRequest.toBuffer() );
			break;
		case FtpReplyCode_230_USER_LOGGED_IN_PROCEED:
			// TODO open data connection here
			break;
		case FtpReplyCode_331_USER_NAME_OKAY_NEED_PASSWORD:
			mFtpRequest.set( FtpCommand_PASS, "****" );
			// TODO get toBuffer to work here
			mText.push_back( mFtpRequest.toString() );
			mConnectionControl->write( mFtpRequest.toBuffer() );
			break;
		default:
			mConnectionControl->close();
			break;
		}
	}
}

CINDER_APP( FtpClientApp, RendererGl )
