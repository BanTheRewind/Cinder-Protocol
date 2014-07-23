#include "BodyInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

BodyInterface::BodyInterface()
: ProtocolInterface()
{
}

void BodyInterface::append( const ci::Buffer& buffer )
{
	size_t sz	= 0;
	size_t len	= buffer.getDataSize();
	if ( mBody ) {
		sz = mBody.getDataSize();
		mBody.resize( sz + len );
	} else {
		mBody = Buffer( len );
	}
	char_traits<char>::copy( (char*)mBody.getData() + sz, (char*)buffer.getData(), len );
}

const Buffer& BodyInterface::getBody() const
{
	return mBody;
}

void BodyInterface::setBody( const Buffer& body )
{
	size_t sz = body.getDataSize();
	if ( mBody && sz > 0 ) {
		mBody.resize( sz );
	} else {
		mBody = Buffer( sz );
	}
	char_traits<char>::copy( (char*)mBody.getData(), (char*)body.getData(), sz );
}

Buffer BodyInterface::toBuffer() const
{
	return mBody;
}

string BodyInterface::toString() const
{
	return bufferToString( mBody );
}
