#include "HttpInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

HttpInterface::HttpInterface( HttpVersion v )
: ProtocolInterface(), mHttpVersion( v )
{
}

HttpVersion HttpInterface::stringToHttpVersion( const string& v )
{
	vector<string> tokens;
	boost::split( tokens, v, boost::is_any_of( "/" ) );
	if ( tokens.size() == 2 ) {
		string ver = tokens.at( 1 );
		if ( ver == "0.9" ) {
			return HTTP_0_9;
		} else if ( ver == "1.0" ) {
			return HTTP_1_0;
		} else if ( ver == "1.1" ) {
			return HTTP_1_1;
		} else if ( ver == "2.0" ) {
			return HTTP_2_0;
		} else {
			throw ExcHttpVersionInvalid( v );
		}
	} else {
		throw ExcHttpVersionInvalid( v );
	}
}

string HttpInterface::httpVersionToString( HttpVersion v )
{
	static const string prefix = "HTTP/";
	switch ( v ) {
		case HTTP_0_9:
			return prefix + "0.9";
		case HTTP_1_0:
			return prefix + "1.0";
		case HTTP_1_1:
			return prefix + "1.1";
		case HTTP_2_0:
			return prefix + "2.0";
		default:
			throw ExcHttpVersionInvalid( ci::toString( v ) );
			break;
	}
}

HttpVersion HttpInterface::getHttpVersion() const
{
	return mHttpVersion;
}

void HttpInterface::setHttpVersion( HttpVersion v )
{
	mHttpVersion = v;
}

HttpInterface::ExcHttpVersionInvalid::ExcHttpVersionInvalid( const string& ver ) throw()
{
	sprintf( mMessage, "\"%s\" is not a valid HTTP version", ver.c_str() );
}
