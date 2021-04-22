#ifndef ENGINE_TRIANGLEAPI_HPP
#define ENGINE_TRIANGLEAPI_HPP

#include "Primitives/Defs.hpp"

MAKE_CENUM_Q(TriCullStyle, i32,
	FRONT, 0,
	NONE, 1,
);

MAKE_CENUM_Q(Tri, i32,
	TRIANGLES, 0,
	TRIANGLE_FAN, 1,
	QUADS, 2,
	POLYGON, 3,
	LINES, 4,
	TRIANGLE_STRIP, 5,
	QUAD_STRIP, 6,
);

struct TriangleAPI
{
	i32 version;
	void (*RenderMode)(i32 mode);
	void (*Begin)(i32 primitiveCode);
	void (*End)(void);

	void (*Color4f)(f32 r, f32 g, f32 b, f32 a);
	void (*Color4ub)(u8 r, u8 g, u8 b, u8 a);
	void (*TexCoord2f)(f32 u, f32 v);
	void (*Vertex3fv)(f32* worldPnt);
	void (*Vertex3f)(f32 x, f32 y, f32 z);
	void (*Brightness)(f32 brightness);
	void (*CullFace)(TriCullStyle style);
	i32 (*SpriteTexture)(struct model_s* pSpriteModel, i32 frame);
	i32 (*WorldToScreen)(f32* world, f32* screen); // Returns 1 if it's z clipped
	void (*Fog)(f32 flFogColor[3], f32 flStart, f32 flEnd, i32 bOn); // Works just like GL_FOG, flFogColor is r/g/b.
	void (*ScreenToWorld)(f32* screen, f32* world);
	void (*GetMatrix)(const i32 pname, f32* matrix);
	i32 (*BoxInPVS)(f32* mins, f32* maxs);
	void (*LightAtPoint)(f32* pos, f32* value);
	void (*Color4fRendermode)(f32 r, f32 g, f32 b, f32 a, i32 rendermode);
	void (*FogParams)(f32 flDensity, i32 iFogSkybox); // Used with Fog()...sets fog density and whether the fog should be applied to the skybox
};

#endif // ENGINE_TRIANGLEAPI_HPP
