#ifndef MAPS_HPP
#define MAPS_HPP

#include "Util.hpp"
#include "Cenum.hpp"

namespace Maps
{
	MAKE_CENUM_Q(Permissions, std::uint8_t,
		NONE, 0,
		READ, 1<<0,
		WRITE, 1<<1,
		EXECUTE, 1<<2,
		PRIVATE, 1<<3,
	);

	struct MapEntry
	{
		std::uintptr_t address = 0;
		std::uintptr_t size = 0;
		Permissions perm = Permissions::NONE;
		long offset = 0;
		char dev[12] = "";
		int inode = 0;
		std::string path = "";
	};

	void ParseMaps(std::function<void(MapEntry&&)> callback);
}

#endif // MAPS_HPP
