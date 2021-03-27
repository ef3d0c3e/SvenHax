#ifndef SDK_ISERVERBROWSER_HPP
#define SDK_ISERVERBROWSER_HPP

#include "../Util/Util.hpp"


class IServerBrowser
{
public:
	virtual ~IServerBrowser();

	// activates the server browser window, brings it to the foreground
	virtual bool Activate() = 0;

	// joins a game directly
	virtual bool JoinGame(u32 unGameIP, u16 usGamePort) = 0;

	// joins a specified game - game info dialog will only be opened if the server is fully or passworded
	virtual bool JoinGame(u64 ulSteamIDFriend) = 0;

	// opens a game info dialog to watch the specified server; associated with the friend 'userName'
	virtual bool OpenGameInfoDialog(u64 ulSteamIDFriend) = 0;

	// forces the game info dialog closed
	virtual void CloseGameInfoDialog(u64 ulSteamIDFriend) = 0;

	// closes all the game info dialogs
	virtual void CloseAllGameInfoDialogs() = 0;
};

#endif // SDK_ISERVERBROWSER_HPP
