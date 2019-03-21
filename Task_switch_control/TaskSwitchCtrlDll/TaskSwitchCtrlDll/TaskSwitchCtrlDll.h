#ifndef TASKSWITCHCTRLDLL_H
#define TASKSWITCHCTRLDLL_H

#ifdef DLLTYPE
#define DLL_EX_IM __declspec(dllexport)
#else
#define DLL_EX_IM __declspec(dllimport)
#endif

extern "C" DLL_EX_IM BOOL TaskSwitching_Control(BOOL Type);
extern "C" DLL_EX_IM BOOL TaskManager_Control(BOOL Type);

#endif