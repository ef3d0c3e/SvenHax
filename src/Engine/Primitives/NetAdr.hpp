#ifndef ENGINE_PRIMITIVES_NETADR_HPP
#define ENGINE_PRIMITIVES_NETADR_HPP

#include "Defs.hpp"

MAKE_CENUM_Q(NetAdrType, int,
	NA_UNUSED, 0,
	NA_LOOPBACK, 1,
	NA_BROADCAST, 2,
	NA_IP, 3,
	NA_IPX, 4,
	NA_BROADCAST_IPX, 5,
);

struct NetAdr
{
	NetAdrType type;
	u8 ip[4];
	u8 ipx[10];
	u16 port;
};

#endif // ENGINE_PRIMITIVES_NETADR_HPP
