#ifndef SDK_CENGINEVGUI_HPP
#define SDK_CENGINEVGUI_HPP

#include "../Util/Cenum.hpp"

MAKE_CENUM_Q(VGUIPANEL, int,
	PANEL_ROOT, 0,
	PANEL_CLIENTDLL, 1,
	PANEL_GAMEUIDLL, 2,
);

MAKE_CENUM_Q(PaintMode_t, int,
	PAINT_UIPANELS, 1<<0,
	PAINT_INGAMEPANELS, 1<<1,
);

MAKE_CENUM_Q(VGuiPanel_t, int,
	PANEL_ROOT, 0,
	PANEL_GAMEUIDLL, 1,  // the console, game menu
	PANEL_CLIENTDLL, 2,
	PANEL_TOOLS, 3,
	PANEL_INGAMESCREENS, 4,
	PANEL_GAMEDLL, 5,
	PANEL_CLIENTDLL_TOOLS, 6,
	PANEL_GAMEUIBACKGROUND, 7, // the console background, shows under all other stuff in 3d engine view
	PANEL_TRANSITIONEFFECT, 8,
	PANEL_STEAMOVERLAY, 9,
);

typedef unsigned int VPANEL;

class CEngineVGui
{
public:
	virtual ~CEngineVGui() = 0;
	virtual VPANEL GetPanel(VGuiPanel_t type) = 0;
	virtual bool SteamRefreshLogin(const char *password, bool isSecure) = 0;
	virtual bool SteamProcessCall(bool *finished, struct TSteamProgress *progress, struct TSteamError *steamError) = 0;
};

#endif // SDK_CENGINEVGUI_HPP
