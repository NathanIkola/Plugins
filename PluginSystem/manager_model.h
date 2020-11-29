//**************************************
// manager_model.h
//
// Holds the definition for the IManager
// model for erasing the type of the
// plugin manager
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************
#pragma once

#include "imanager.h"

namespace Plugin
{
	//**********************************
	// Derived class that knows what
	// type it is erasing and can
	// call functions by the
	// interface it exposes
	//**********************************
	template<typename Type>
	struct ManagerModel final : ManagerConcept
	{
		//******************************
		// ctor grabs a reference to
		// the item passed in and
		// holds on to it for
		// dispatching later
		//******************************
		inline ManagerModel(Type& manager) noexcept : m_manager(manager) { }

		//******************************
		// Final passthrough Register
		// function, used for
		// extracting the final
		// behavior from the erased
		// type that it contains
		//******************************
		virtual inline void Register(const char* handle, void* function) const noexcept override
		{
			m_manager.Register(handle, function);
		}

		//******************************
		// Final passthrough Unregister
		// function, used for removing
		// the final behavior when this
		// plugin is unloaded
		//******************************
		virtual inline void Unregister(const char* handle, void* function) const noexcept override
		{
			m_manager.Unregister(handle, function);
		}

		//******************************
		// Final passthrough 
		// Function getter, used for
		// getting existing funcs
		// from other plugins that
		// are registered with
		// the plugin manager
		//******************************
		virtual std::vector<void*> PluginFunctions(const char* handle) const noexcept override
		{
			return m_manager.GetPluginFuncs(handle);
		}

		// the type to be erased
		Type& m_manager;
	};
}
