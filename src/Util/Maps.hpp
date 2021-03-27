#ifndef MAPS_HPP
#define MAPS_HPP

#include "Util.hpp"
#include "Cenum.hpp"

namespace Maps
{
	MAKE_CENUM_Q(Permissions, u8,
		NONE, 0,
		READ, 1<<0,
		WRITE, 1<<1,
		EXECUTE, 1<<2,
		PRIVATE, 1<<3,
	);

	struct maps_entry_t
	{
		std::uintptr_t address = 0;
		std::uintptr_t size = 0;
		Permissions perm = Permissions::NONE;
		long offset = 0;
		char dev[12] = "";
		int inode = 0;
		std::string path = "";
	};
	extern std::vector<maps_entry_t> mapsEntries;

	void ParseMaps();
}

#endif // MAPS_HPP
