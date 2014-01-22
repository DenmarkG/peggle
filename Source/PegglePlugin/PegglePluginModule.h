#ifndef GAMEPLUGINMODULE_H_INCLUDED
#define GAMEPLUGINMODULE_H_INCLUDED

#ifdef WIN32
  #ifdef SAMPLEPLUGIN_EXPORTS
    #define PEGGLE_PLUGIN_IMPEXP __declspec(dllexport)
  #elif SAMPLEPLUGIN_IMPORTS
    #define PEGGLE_PLUGIN_IMPEXP __declspec(dllimport)
  #else
    #define PEGGLE_PLUGIN_IMPEXP __declspec()
  #endif

#elif defined (_VISION_IOS) || defined(_VISION_ANDROID) || defined(HK_PLATFORM_TIZEN)
  #define PEGGLE_PLUGIN_IMPEXP

#else
  #error Undefined platform!
#endif

#endif //  GAMEPLUGINMODULE_H_INCLUDED