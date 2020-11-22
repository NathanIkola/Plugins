//**************************************
// plugin_manager.h
//
// Holds the declaration for my plugin
// manager system
//
// This is a singleton for a reason:
//	- It will control concurrent
//	  access to the plugin system
//	- It will be used by discrete
//	  parts of the program
//	- There should be only one
//	  plugin manager in use
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************
#pragma once

#include <assert.h>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "imanager.h"
#include "plugin_handle.h"

#define DLLVERSION	"dll_version"
#define DLLENTRY	"dll_register"
#define DLLEXIT		"dll_unregister"

// the lowest version number that can
// still be loaded for this application
#define MIN_VERSION 0.0

namespace Plugin
{
	// the version is represented by a float: the integral
	// portion is the major number, and the fractional
	// portion is the minor number
	using version = float;

	class PluginManager final
	{
	public:
		//************************************
		// Static instance getter
		//************************************
		static inline PluginManager& GetInstance() noexcept
		{
			static PluginManager _pm;
			return _pm;
		}

		//************************************
		// Load in the plugin at filename
		//************************************
		void LoadPlugin(const char* filename) noexcept;

		//************************************
		// Unload the specified plugin
		//************************************
		void UnloadPlugin(void* plugin) noexcept;

		//************************************
		// Default destructor is acceptable
		//************************************
		inline ~PluginManager() noexcept 
		{
			while (m_plugins.size() != 0)
			{
				auto first = m_plugins.begin();
				UnloadPlugin(*(first));
				m_plugins.erase(first);
			}
		}

		//************************************
		// Plugin register function
		//************************************
		inline void Register(const char* handle, void* func) noexcept
		{
			if (m_handles.find(handle) == m_handles.end())
				m_handles.insert({ handle, func });
			else
				m_handles[handle].Attach(func);
		}

		//************************************
		// Plugin unregister function
		//************************************
		inline void Unregister(const char* handle, void* func) noexcept
		{
			assert(m_handles.find(handle) != m_handles.end());
			m_handles[handle].Detach(func);
		}

		//************************************
		// Handle getter
		//************************************
		inline const PluginHandle& GetHandle(const char* handle) noexcept
		{
			if (m_handles.find(handle) == m_handles.end())
				m_handles[handle] = {};
			return m_handles.at(handle);
		}

		//************************************
		// Static handle getter
		//************************************
		template<typename FuncPtr>
		static inline std::vector<FuncPtr> GetPluginFuncs(const char* handle) noexcept
		{
			PluginManager& p = GetInstance();
			return p.GetHandle(handle).As<FuncPtr>();
		}

		//************************************
		// Handle getter
		//************************************
		inline std::vector<void*> GetPluginFuncs(const char* handle) noexcept
		{
			return GetHandle(handle).Pointers();
		}

		//************************************
		// Cast to IManager
		//************************************
		inline operator IManager() { return IManager(*this); }

		//************************************
		// Execute all plugin functions for
		// a specific handle (ref version)
		//************************************
		template<typename T>
		inline T ExecutePlugins(const char* handle, T data)
		{
			std::vector<T(*)(T)> _f = GetPluginFuncs<T(*)(T)>(handle);
			for (auto f : _f)
				data = f(data);
			return data;
		}
	private:
		//************************************
		// Default constructor is acceptable
		//************************************
		inline PluginManager() noexcept {}

		//************************************
		// Explicitly delete copy operations
		//************************************
		inline PluginManager(const PluginManager&) = delete;
		inline PluginManager(PluginManager&&) = delete;
		inline PluginManager& operator=(const PluginManager&) = delete;
		inline PluginManager& operator=(PluginManager&&) = delete;

		std::unordered_map<std::string, PluginHandle> m_handles = {};

		// the vector of handles to free when
		// the plugin manager is destoryed
		std::vector<void*> m_plugins = {};
	};

	// alias plugin manager for convenience
	using PMgr = PluginManager;
}