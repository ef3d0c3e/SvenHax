#ifndef ENGINE_PRIMITIVES_DCLIPNODE_HPP
#define ENGINE_PRIMITIVES_DCLIPNODE_HPP

struct DClipNode
{
	i32 planenum;
	i16 children[2]; // negative numbers are contents
};

#endif // ENGINE_PRIMITIVES_DCLIPNODE_HPP
