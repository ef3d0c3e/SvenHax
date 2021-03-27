#ifndef SDK_IENGINEVGUI_HPP
#define SDK_IENGINEVGUI_HPP

#include "../Util/Cenum.hpp"
#include "IBaseInterface.hpp"
#include "CEngineVGui.hpp"


class IEngineVGui : public IBaseInterface
{
public:
	virtual ~IEngineVGui(void) { }
	virtual VPANEL GetPanel(VGUIPANEL type) = 0;
	virtual bool SteamRefreshLogin(const char *password, bool isSecure) = 0;
	virtual bool SteamProcessCall(bool *finished, struct TSteamProgress *progress, struct TSteamError *steamError) = 0;
};

#endif // SDK_IENGINEVGUI_HPP
