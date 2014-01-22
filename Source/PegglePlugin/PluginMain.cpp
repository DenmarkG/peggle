#include "PegglePluginPCH.h"
#include "PeggleGameManager.h"
#include "SimplePeg.h"

#include <Common/Base/KeyCode.h>

// use plugins if supported
VIMPORT IVisPlugin_cl* GetEnginePlugin_vFmodEnginePlugin();

#if defined( HAVOK_PHYSICS_2012_KEYCODE )
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavok();
#endif

#if defined( HAVOK_AI_KEYCODE )
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavokAi();
#endif

#if defined( HAVOK_BEHAVIOR_KEYCODE )
VIMPORT IVisPlugin_cl* GetEnginePlugin_vHavokBehavior();
#endif

class PegglePluginClass : public IVisPlugin_cl
{
public:
	void OnInitEnginePlugin();    
	void OnDeInitEnginePlugin();  

	const char *GetPluginName()
	{
		// must match DLL name
		return "PegglePlugin";
	}
};

PegglePluginClass g_pegglePluginClassInstance;

DECLARE_THIS_MODULE(g_peggleModule, MAKE_VERSION(1,0),
					//#todo #bettercomment
                    "Simple Plugin", 
						//#todo #yournamebro
                    "Havok",
					//#todo #bettercomment
                    "A simple plugin to allow use of the Simple Peg component", &g_pegglePluginClassInstance);

//--------------------------------------------------------------------------------------------
//  Use this to get and initialize the plugin when you link statically
//--------------------------------------------------------------------------------------------
VEXPORT IVisPlugin_cl* GetEnginePlugin_PegglePlugin()
{
	return &g_pegglePluginClassInstance;
}

#if (defined _DLL) || (defined _WINDLL)

  //  The engine uses this to get and initialize the plugin dynamically
  VEXPORT IVisPlugin_cl* GetEnginePlugin()
  {
	  return GetEnginePlugin_PegglePlugin();
  }

#endif // _DLL or _WINDLL
//  Called when the plugin is loaded
//  We add our component initialize data here
void PegglePluginClass::OnInitEnginePlugin()
{
	hkvLog::Info("PegglePluginClass:OnInitEnginePlugin()");
	Vision::RegisterModule(&g_peggleModule);

	// load plugins if supported
#if defined( HAVOK_PHYSICS_2012_KEYCODE )
	VISION_PLUGIN_ENSURE_LOADED(vHavok);
#endif
#if defined( HAVOK_AI_KEYCODE )
	VISION_PLUGIN_ENSURE_LOADED(vHavokAi);
#endif
#if defined( HAVOK_BEHAVIOR_KEYCODE )
	VISION_PLUGIN_ENSURE_LOADED(vHavokBehavior);
#endif

	VISION_PLUGIN_ENSURE_LOADED(vFmodEnginePlugin);

	// Start our component managers and game manager here....
	PeggleGameManager::GlobalManager().OneTimeInit();
	// [...]

	//---------------------------------------------------------------------------------------------------------
	// register the action module with the vision engine action manager
	// only after that will the action become available in the console.
	//---------------------------------------------------------------------------------------------------------
	VActionManager * pManager = Vision::GetActionManager ();
	pManager->RegisterModule ( &g_peggleModule );

	// Set to true to open the console at startup and print some data to the display
	Vision::GetConsoleManager()->Show( false );
}

// Called before the plugin is unloaded
void PegglePluginClass::OnDeInitEnginePlugin()
{
	hkvLog::Info("PegglePluginClass:OnDeInitEnginePlugin()");
    
	// Close our component managers here....
	PeggleGameManager::GlobalManager().OneTimeDeInit();
	//  [...]
  
	// de-register component from action manager
	VActionManager * pManager = Vision::GetActionManager ();
	pManager->UnregisterModule( &g_peggleModule );

	// de-register our module when the plugin is de-initialized
	Vision::UnregisterModule(&g_peggleModule);
}
