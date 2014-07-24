#include "CommandInterface.h"

#include "boost/algorithm/string.hpp"
#include "cinder/Utilities.h"
#include <vector>

using namespace ci;
using namespace std;

CommandInterface::CommandInterface()
: ProtocolInterface()
{
}

Buffer CommandInterface::toBuffer() const
{
	Buffer buffer;
	return buffer;
}

string CommandInterface::toString() const
{
	return "";
}

CommandInterface::ExcCommandNotFound::ExcCommandNotFound( const string& command ) throw()
{
	sprintf( mMessage, "Command \"%s\" not found", command.c_str() );
}

ostream& operator<<( ostream& out, const CommandInterface& p )
{
	out << p.toString();
	return out;
}
