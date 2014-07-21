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
	void						onReadComplete();
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
	if ( mServer ) {
		mServer->accept( 80 );
		mText.push_back( "Listening on port: 80" );
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

void HttpServerApp::onClose()
{
	mText.push_back( "Disconnected" );
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
	
	if ( !mHttpResponse.hasHeader() ) {
		mHttpResponse.parseHeader( HttpResponse::bufferToString( buffer ) );
		buffer = HttpResponse::removeHeader( buffer );
	}
	mHttpResponse.append( buffer );
	mSession->read();
}

void HttpServerApp::onReadComplete()
{
	mText.push_back( "Read complete" );
		
	console() << "HTTP version: ";
	switch ( mHttpResponse.getHttpVersion() ) {
		case HttpVersion::HTTP_0_9:
			console() << "0.9";
			break;
		case HttpVersion::HTTP_1_0:
			console() << "1.0";
			break;
		case HttpVersion::HTTP_1_1:
			console() << "1.1";
			break;
		case HttpVersion::HTTP_2_0:
			console() << "2.0";
			break;
	}
	console() << endl;
	console() << "Status code: " << mHttpResponse.getStatusCode() << endl;
	console() << "Reason: " << mHttpResponse.getReason() << endl;
	
	console() << "Headers: " << endl;
	for ( const KeyValuePair& kvp : mHttpResponse.getHeaders() ) {
		console() << ">> " << kvp.first << ": " << kvp.second << endl;
	}
	console() << endl;
	
	console() << "Response buffer:" << endl;
	console() << mHttpResponse << endl;
	
	mSession->close();
}

void HttpServerApp::onWrite( size_t bytesTransferred )
{
	mText.push_back( toString( bytesTransferred ) + " bytes written" );
	
	mSession->read();
}

void HttpServerApp::setup()
{
	gl::enable( GL_TEXTURE_2D );
	
	mFont			= Font( "Georgia", 60 );
	mFrameRate		= 0.0f;
	mFullScreen		= false;
	
	mParams = params::InterfaceGl::create( "Params", Vec2i( 200, 150 ) );
	mParams->addParam( "Frame rate",	&mFrameRate,					"", true );
	mParams->addParam( "Full screen",	&mFullScreen,					"key=f" );
	mParams->addButton( "Quit", bind(	&HttpServerApp::quit, this ),	"key=q" );
}

void HttpServerApp::update()
{
	mFrameRate = getFrameRate();
	
	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
		mFullScreen = isFullScreen();
	}

	if ( !mText.empty() ) {
		TextBox tbox = TextBox().alignment( TextBox::LEFT ).font( mFont ).size( Vec2i( getWindowWidth() - 250, TextBox::GROW ) ).text( "" );
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
