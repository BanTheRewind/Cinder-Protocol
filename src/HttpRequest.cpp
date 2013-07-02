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

void HttpRequest::parseRequestLine( const string& requestLine )
{
	string status = boost::trim_copy( requestLine );
	vector<string> tokens;
	boost::split( tokens, status, boost::is_any_of( " " ) );
	if ( tokens.size() != 3 ) {
		throw ExcRequestLineInvalid( requestLine );
	} else {
		mOptions		= tokens.at( 0 );
		mUri			= tokens.at( 1 );
		mHttpVersion	= stringToHttpVersion( tokens.at( 2 ) );
	}
}

string HttpRequest::headerToString() const
{
	string header	= mOptions + " " + mUri + " ";
	header			+= httpVersionToString( mHttpVersion );
	if ( !mHeaderMap.empty() ) {
		header		+= sCrLf;
		header		+= headerMapToString( mHeaderMap );
	}
	return header;
}

HttpRequest::ExcRequestLineInvalid::ExcRequestLineInvalid( const string& requestLine ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid request line", requestLine.c_str() );
}
