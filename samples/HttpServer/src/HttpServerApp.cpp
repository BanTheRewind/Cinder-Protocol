#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"
#include "cinder/Text.h"

#include "boost/algorithm/string.hpp"

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "TcpServer.h"

class HttpServerApp : public ci::app::AppBasic 
{
public:
	void						draw();
	void						setup();
	void						update();
private:
	void						accept();
	TcpServerRef				mServer;
	TcpSessionRef				mSession;
	
	HttpRequest					mHttpRequest;
	HttpResponse				mHttpResponse;
	
	void						onAccept( TcpSessionRef session );
	void						onCancel();
	void						onClose();
	void						onError( std::string err, size_t bytesTransferred );
	void						onRead( ci::Buffer buffer );
	void						onWrite( size_t bytesTransferred );
	
	ci::Font					mFont;
	std::vector<std::string>	mText;
	ci::gl::TextureRef			mTexture;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;
};

#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void HttpServerApp::accept()
{
	
	// Ports <1024 are restricted to root
	if ( mServer ) {
		mServer->accept( 2000 );
		mText.push_back( "Listening on port: 2000" );
	}
}

void HttpServerApp::draw()
{
	gl::clear( Colorf::black() );
	gl::setMatricesWindow( getWindowSize() );
	
	if ( mTexture ) {
		gl::draw( mTexture, Vec2i( 250, 20 ) );
	}
	
	mParams->draw();
}

void HttpServerApp::onAccept( TcpSessionRef session )
{
	mHttpRequest	= HttpRequest();
	mSession		= session;
	mText.push_back( "Connected" );
	
	mSession->connectCloseEventHandler( [ & ]() {
		mText.push_back( "Session closed" );
	} );
	mSession->connectErrorEventHandler( &HttpServerApp::onError, this );
	mSession->connectReadEventHandler( &HttpServerApp::onRead, this );
	mSession->connectWriteEventHandler( &HttpServerApp::onWrite, this );
	
	mSession->read();
}

void HttpServerApp::onCancel()
{
	mText.push_back( "Canceled" );
	
	accept();
}

void HttpServerApp::onClose()
{
	mText.push_back( "Disconnected" );
	
	accept();
}

void HttpServerApp::onError( string err, size_t bytesTransferred )
{
	string text = "Error";
	if ( !err.empty() ) {
		text += ": " + err;
	}
	mText.push_back( text );
}

void HttpServerApp::onRead( ci::Buffer buffer )
{
	mText.push_back( toString( buffer.getDataSize() ) + " bytes read" );
	
	// Parse the request buffer into headers and body
	mHttpRequest.parse( buffer );
	
	console() << mHttpRequest << endl;

	// We're expecting a number between 0 and 2 from the client
	string request	= HttpRequest::bufferToString( mHttpRequest.getBody() );
	int32_t index	= -1;
	try {
		index = fromString<int32_t>( request );
	} catch ( boost::bad_lexical_cast ) {
		console() << "Unable to cast to int32_t" << endl;
	}
	
	// Start a new response
	mHttpResponse = HttpResponse();
	mHttpResponse.setHeader( "Server", "Cinder" );
	mHttpResponse.setHttpVersion( HttpVersion::HTTP_1_1 );
	
	// Declare an empty response body
	Buffer body;

	if ( index < 0 || index > 2 ) {
		
		// The request was invalid, so let's create an error
		mHttpResponse.setHeader( "Content-Type", "text/html; charset=uhf-8" );
		mHttpResponse.setReason( "Internal Server Error" );
		mHttpResponse.setStatusCode( 500 );
		
		// Set the message body
		string msg	= "\"" + request + "\" is invalid";
		body		= HttpResponse::stringToBuffer( msg );
		mHttpResponse.setBody( body );
	} else {
		
		// Request was OK
		mHttpResponse.setReason( "OK" );
		mHttpResponse.setStatusCode( 200 );
		
		// Let's pick out a mime type and file based on the requested index
		fs::path path;
		string mime = "";
		switch ( index ) {
			case 0:
				mime = "image/jpeg";
				path = getAssetPath( request + ".jpg" );
				break;
			case 1:
				mime = "image/png";
				path = getAssetPath( request + ".png" );
				break;
			case 2:
				mime = "audio/mp3";
				path = getAssetPath( request + ".mp3" );
				break;
		}
		mHttpResponse.setHeader( "Content-Type", mime );
		
		// Load the file from assets into a buffer
		DataSourceRef file	= loadFile( path );
		body				= file->getBuffer();
	}
	
	// Set the content length using the body size
	mHttpResponse.setBody( body );
	size_t sz = body.getDataSize();
	mHttpResponse.setHeader( "Content-Length", toString( sz ) );
	
	// Tell the client to close the connection when they're finished
	mHttpResponse.setHeader( "Connection", "close" );
	
	// Send the response back to the client
	mSession->write( mHttpResponse.toBuffer() );
}

void HttpServerApp::onWrite( size_t bytesTransferred )
{
	mText.push_back( toString( bytesTransferred ) + " bytes written" );

	mSession->close();
	accept();
}

void HttpServerApp::setup()
{
	gl::enable( GL_TEXTURE_2D );
	
	mFont			= Font( "Georgia", 24 );
	mFrameRate		= 0.0f;
	mFullScreen		= false;
	
	mParams = params::InterfaceGl::create( "Params", Vec2i( 200, 150 ) );
	mParams->addParam( "Frame rate",	&mFrameRate,					"", true );
	mParams->addParam( "Full screen",	&mFullScreen,					"key=f" );
	mParams->addButton( "Quit", bind(	&HttpServerApp::quit, this ),	"key=q" );
	
	mServer = TcpServer::create( io_service() );
	
	mServer->connectAcceptEventHandler( &HttpServerApp::onAccept, this );
	mServer->connectCancelEventHandler( &HttpServerApp::onCancel, this );
	mServer->connectErrorEventHandler( &HttpServerApp::onError, this );
	
	accept();
}

void HttpServerApp::update()
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

CINDER_APP_BASIC( HttpServerApp, RendererGl )
