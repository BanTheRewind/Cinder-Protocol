#include "HttpRequest.h"

#include "boost/algorithm/string.hpp"

using namespace ci;
using namespace std;

HttpRequest::HttpRequest()
: HttpInterface( HttpVersion::HTTP_1_1 ), mOptions( "GET" ), mUri( "*" )
{
}

HttpRequest::HttpRequest( const string& options, const string& uri, HttpVersion httpVersion )
: HttpInterface( httpVersion ), mOptions( options ), mUri( uri )
{
}

const string& HttpRequest::getOptions() const
{
	return mOptions;
}

void HttpRequest::setOptions( const string& options )
{
	mOptions = options;
}

const string& HttpRequest::getUri() const
{
	return mUri;
}

void HttpRequest::setUri( const string& uri )
{
	mUri = uri;
}

void HttpRequest::parseHeader( const string& header )
{
	vector<string> lines;
	boost::split( lines, header, boost::is_any_of( sCrLf ) );
	if ( !lines.empty() ) {
		string request = boost::trim_copy( lines.at( 0 ) );
		vector<string> tokens;
		boost::split( tokens, request, boost::is_any_of( " " ) );
		if ( tokens.size() < 2 ) {
			throw ExcRequestLineInvalid( request );
		} else {
			mOptions		= tokens.at( 0 );
			mUri			= tokens.at( 1 );
			mHttpVersion	= stringToHttpVersion( tokens.at( 2 ) );
		}
		lines.erase( lines.begin() );
	}
	
	string headerMap	= boost::join( lines, sCrLf );
	mHeaderMap			= stringToHeaderMap( headerMap );
	
	mHasHeader			= true;
}

string HttpRequest::headerToString() const
{
	string header	= mOptions + " " + mUri + " ";
	header			+= httpVersionToString( mHttpVersion );
	header			+= sCrLf;
	if ( !mHeaderMap.empty() ) {
		header		+= headerMapToString( mHeaderMap );
	}
	header			+= sCrLf;
	return header;
}

HttpRequest::ExcRequestLineInvalid::ExcRequestLineInvalid( const string& requestLine ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid request line", requestLine.c_str() );
}
