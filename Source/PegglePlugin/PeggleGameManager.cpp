#include "PegglePluginPCH.h"

#include "PeggleGameManager.h"

PeggleGameManager PeggleGameManager::g_GameManager;

void PeggleGameManager::OneTimeInit()
{
	m_bPlayingTheGame = false;

	// The game manager should listen to the following callbacks:
	Vision::Callbacks.OnEditorModeChanged += this;
	Vision::Callbacks.OnBeforeSceneLoaded += this;
	Vision::Callbacks.OnAfterSceneLoaded += this;
	Vision::Callbacks.OnUpdateSceneBegin += this;
	Vision::Callbacks.OnWorldDeInit += this;
}

void PeggleGameManager::OneTimeDeInit()
{
	Vision::Callbacks.OnEditorModeChanged -= this;
	Vision::Callbacks.OnBeforeSceneLoaded -= this;
	Vision::Callbacks.OnAfterSceneLoaded -= this;
	Vision::Callbacks.OnUpdateSceneBegin -= this;
	Vision::Callbacks.OnWorldDeInit -= this;
}

void PeggleGameManager::OnHandleCallback(IVisCallbackDataObject_cl *pData)
{
	if (pData->m_pSender==&Vision::Callbacks.OnUpdateSceneBegin)
	{
		// This callback will be triggered at the beginning of every frame
		// You can add your own per frame logic here
	}
	else if (pData->m_pSender==&Vision::Callbacks.OnEditorModeChanged)
	{
		// When vForge switches back from EDITORMODE_PLAYING_IN_GAME, turn off our play the game mode
		if ( ((VisEditorModeChangedDataObject_cl *)pData)->m_eNewMode != VisEditorManager_cl::EDITORMODE_PLAYING_IN_GAME )
		{
			SetPlayTheGame(false);
		}
	}
	else if (pData->m_pSender==&Vision::Callbacks.OnBeforeSceneLoaded)
	{
		// Here you can add you specific code before the scene is loaded
	}
	else if (pData->m_pSender==&Vision::Callbacks.OnAfterSceneLoaded)
	{
		// Gets triggered when the play-the-game vForge is started or outside vForge after loading the scene
		if ( Vision::Editor.IsPlayingTheGame() )
		{
			SetPlayTheGame(true);
		}
	}
	else if (pData->m_pSender==&Vision::Callbacks.OnWorldDeInit) 
	{
		// This is important when running outside vForge
		SetPlayTheGame(false);
	}
}

void PeggleGameManager::SetPlayTheGame(bool bStatus)
{
	if (m_bPlayingTheGame != bStatus)
	{
		m_bPlayingTheGame = bStatus;

		//if (m_bPlayingTheGame)
		//{
		//}
	}
}