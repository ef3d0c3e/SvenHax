#ifndef HACKS_TEMP_HPP
#define HACKS_TEMP_HPP

#include "../Interface.hpp"

namespace Temp
{
	// gSharedString
	std::string address;
	u16 port;
	std::string map;
	// TODO: Call on level/server change
	bool parseSharedString();
}

#endif // HACKS_TEMP_HPP
