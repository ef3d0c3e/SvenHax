#ifndef SDK_CENGINECLIENT_HPP
#define SDK_CENGINECLIENT_HPP

#include "../Util/Util.hpp"
#include <stdarg.h>

struct RenderTarget;
struct ref_params_s;

class CEngineClient
{
public:
	virtual ~CEngineClient() = 0;

	virtual void Con_DnPrintf(int, char*, ...) = 0;
	virtual void DrawCharacterOpq(int, int, int, int, int, int, int) = 0; // char (i), color (iii), pos (ii), font index (i) ???
	virtual void DrawCharacterOpq(int, int, int, int, int, int, unsigned int) = 0; // char (i), color (iii), pos (ii), font index (j) ???
	virtual bool /*?*/ LoadClientModel(const char*) = 0;
	virtual bool /*?*/ ChangeSkymap(const char*) = 0;
	virtual bool /*?*/ QueueDemoSound(float*, int, char const*, float, float, int, int) = 0;
	virtual void PushView(RenderTarget*, bool, bool) = 0;
	virtual void RenderView(ref_params_s*, bool, bool, int) = 0;
};

#endif // SDK_CENGINECLIENT_HPP
