#include "HttpResponse.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"

using namespace ci;
using namespace std;

HttpResponse::HttpResponse()
: HttpInterface( HttpVersion::HTTP_1_1 ), mReason( "" ), mStatusCode( 0 )
{
}

HttpResponse::HttpResponse( HttpVersion httpVersion, size_t statusCode, const string& reason )
: HttpInterface( HttpVersion::HTTP_1_1 ), mReason( reason ), mStatusCode( statusCode )
{
}

const string& HttpResponse::getReason() const
{
	return mReason;
}

void HttpResponse::setReason( const string& reason )
{
	mReason = reason;
}

size_t HttpResponse::getStatusCode() const
{
	return mStatusCode;
}

void HttpResponse::setStatusCode( size_t code )
{
	mStatusCode = code;
}

void HttpResponse::parseResponse( const string& response )
{
	vector<string> tokens;
	boost::split( tokens, response, boost::is_any_of( sCrLf ) );
	string body = "";
	for ( size_t i = 0; i < tokens.size(); ++i ) {
		const string& v = tokens.at( i );
		switch ( i ) {
			case 0:
				parseStatusLine( v );
				break;
			case 1:
				mHeaderMap = stringToHeaderMap( v );
				break;
			default:
				body += v;
				break;
		}
	}
	//mBody = stringToBuffer( body );
}

void HttpResponse::parseStatusLine( const string& statusLine )
{
	string status = boost::trim_copy( statusLine );
	vector<string> tokens;
	boost::split( tokens, status, boost::is_any_of( " " ) );
	if ( tokens.size() < 2 ) {
		throw ExcStatusLineInvalid( statusLine );
	} else {
		mHttpVersion	= stringToHttpVersion( tokens.at( 0 ) );
		mStatusCode		= fromString<size_t>( tokens.at( 1 ) );
		mReason			= "";
		for ( size_t i = 0; i < 2; ++i ) {
			tokens.erase( tokens.begin() );
		}
		if ( !tokens.empty() ) {
			mReason		= boost::join( tokens, " " );
		}
	}
}

string HttpResponse::headerToString() const
{
	string header	= httpVersionToString( mHttpVersion );;
	header			+= " " + ci::toString( mStatusCode ) + " " + mReason;
	header			+= sCrLf;
	if ( !mHeaderMap.empty() ) {
		header		+= headerMapToString( mHeaderMap );
		header		+= sCrLf;
	}
	return header;
}

HttpResponse::ExcStatusLineInvalid::ExcStatusLineInvalid( const string& statusLine ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid status line", statusLine.c_str() );
}
