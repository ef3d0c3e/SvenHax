#include "Chams.hpp"
#include "Info.hpp"
#include <GL/gl.h>

bool Settings::Chams::enabled = true;

bool Chams::StudioRenderModel()
{
	Entity* ent = gStudioApi->GetCurrentEntity();
	if (!ent || ent->player)
		return false;

	auto it = Info::entityInfo.find(ent->index);
	if (it == Info::entityInfo.end())
		return false;

	const Info::EntInfo einfo = it->second;
	/*
	einfo.ent.curstate.renderMode = 0;
	einfo.ent.curstate.renderFx = 0;
	einfo.ent.curstate.renderAmt = 0;
	gStudioApi->SetForceFaceFlags(0);

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Walls
	{
		glDepthFunc(GL_GREATER);
		glDisable(GL_DEPTH_TEST);

		gStudioRenderer->StudioRenderFinal_Hardware();
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	*/
	//gStudioRenderer->StudioRenderFinal_Software();
	//TODO

	//glEnable(GL_TEXTURE_2D);

	return true;
}
