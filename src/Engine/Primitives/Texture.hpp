#ifndef ENGINE_PRIMITIVES_TEXTURE_HPP
#define ENGINE_PRIMITIVES_TEXTURE_HPP

struct Texture
{
	static constexpr inline std::size_t NameLen = 16;
	char name[NameLen];
	u16 width, height;
	i32 animTotal; // total tenths in sequence ( 0 = no)
	i32 animMin, animMax; // time for this frame min <=time< max
	Texture* animNext; // in the animation sequence
	Texture* alternateAnims; // bmodels in frame 1 use these
	static constexpr inline std::size_t MipLevels = 4;
	u32 offsets[MipLevels]; // four mip maps stored
	u32 palOffset;
};

#endif // ENGINE_PRIMITIVES_TEXTURE_HPP
