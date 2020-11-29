//************************************
// plugin_manager.cpp
//
// Holds the implementation for
// the plugin manager
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//************************************
#include "plugin_manager.h"

#include <Windows.h>
#include "manager_model.h"

//************************************
// Load in the plugin at filename
//************************************
void Plugin::PluginManager::LoadPlugin(const char* filename) noexcept
{
	// double check that the filename is a DLL
	uint8_t state = 0;
	const char* cursor = filename;
	while (*cursor != '\0')
	{
		if (*cursor == 'd' && state == 0) ++state;
		else if (*cursor == 'l' && (state == 1 || state == 2)) ++state;
		else state = 0;
		++cursor;
	}
	// did not point to a DLL
	assert(state == 3);

	// now import the dll
	HINSTANCE dll = LoadLibraryA(filename);
	assert(dll != nullptr);

	// load the methods we need
	FARPROC _dllVersion = GetProcAddress(dll, DLLVERSION);
	FARPROC _dllEntry = GetProcAddress(dll, DLLENTRY);

	// check that the version matches
	bool (*dllVersion)(float) = reinterpret_cast<bool (*)(float)>(_dllVersion);
	// ensure the plugin is compatible
	assert(dllVersion(VERSION));

	// now that we know it is good, add it to our list
	// of loaded plugins
	m_plugins.push_back(dll);

	// run the register method
	void (*dllEntry)(IManager) = reinterpret_cast<void (*)(IManager)>(_dllEntry);
	dllEntry(IManager(new ManagerModel<PMgr>(PMgr::GetInstance())));
}

//**************************************
// Unload the plugins that have been
// regsistered 
//**************************************
void Plugin::PluginManager::UnloadPlugin(void* plugin) noexcept
{
	// cast our plugin back into an HINSTANCE
	HINSTANCE dll = static_cast<HINSTANCE>(plugin);

	// run this plugin's cleanup method
	FARPROC _dllExit = GetProcAddress(dll, DLLEXIT);
	assert(_dllExit != nullptr);
	reinterpret_cast<void(*)(IManager)>(_dllExit)(IManager(new ManagerModel<PMgr>(PMgr::GetInstance())));

	// make sure the plugin was successfully freed
	assert(FreeLibrary(dll));
}