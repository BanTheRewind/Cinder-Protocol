#include "KeyValuePairInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

KeyValuePairInterface::KeyValuePairInterface( const string& key, const string& value )
: ProtocolInterface(), mKeyValuePair( make_pair( key, value ) )
{
}

const string& KeyValuePairInterface::getValue() const
{
	return mKeyValuePair.second;
}

void KeyValuePairInterface::setValue( const string& v )
{
	mKeyValuePair.second = v;
}

void KeyValuePairInterface::parse( const Buffer& buffer )
{
	string s = bufferToString( buffer );
	vector<string> lines;
	boost::split( lines, s, boost::is_any_of( sCrLf ) );
	if ( !lines.empty() ) {
		vector<string> pair;
		boost::split( pair, lines.at( 0 ), boost::is_any_of( " " ) );
		if ( pair.size() > 1 ) {
			string key		= pair.at( 0 );
			pair.erase( pair.begin() );
			string value	= boost::join( pair, " " );
			mKeyValuePair	= make_pair( key, value );
		}
	}
}

Buffer KeyValuePairInterface::toBuffer() const
{
	Buffer buffer;
	if ( !mKeyValuePair.first.empty() ) {
		string s = keyValuePairToString( mKeyValuePair );
		buffer = stringToBuffer( s );
	}
	return buffer;
}

string KeyValuePairInterface::toString() const
{
	return keyValuePairToString( mKeyValuePair );
}

ostream& operator<<( ostream& out, const KeyValuePairInterface& p )
{
	out << p.toString();
	return out;
}
 