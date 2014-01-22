#ifndef PLUGINMANAGER_H_INCLUDED
#define PLUGINMANAGER_H_INCLUDED

#include "PegglePluginModule.h"

class PeggleGameManager : public IVisCallbackHandler_cl
{
public:
	virtual void OnHandleCallback(IVisCallbackDataObject_cl *pData) HKV_OVERRIDE;

	// called when plugin is loaded/unloaded
	void OneTimeInit();
	void OneTimeDeInit();

	// switch to play-the-game mode. When not in vForge, this is default
	void SetPlayTheGame(bool bStatus);

	// access one global instance of the Game manager
	static PeggleGameManager& GlobalManager()
	{
		return g_GameManager;
	}

private:
	bool m_bPlayingTheGame;

	static PeggleGameManager g_GameManager;
};

#endif // PLUGINMANAGER_H_INCLUDED