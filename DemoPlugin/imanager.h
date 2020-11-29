//**************************************
// imanager.h
//
// Holds the definition for a type
// erased container for PluginManager
// for use in the plugin DLLs
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************
#pragma once

// required for std::shared_ptr
#include <memory>
// required for std::vector
#include <vector>

// the version of the plugin manager this was designed for
#define VERSION		0.0;

namespace Plugin
{
	//**********************************
	// Abstract base class, used for
	// erasing the type contained in
	// the child class ManagerModel
	//**********************************
	struct ManagerConcept
	{
		// IOC Register method
		virtual void Register(const char*, void*) const noexcept = 0;
		// IOC Unregister method
		virtual void Unregister(const char*, void*) const noexcept = 0;
		// IOC Function getter
		virtual std::vector<void*> PluginFunctions(const char* handle) const noexcept = 0;
	};

	//**********************************
	// IManager encapsulates the
	// PluginManager interface required
	// to initialize plugins without
	// requiring a full definition of
	// the PluginManager class by
	// using type erasure
	//**********************************
	class IManager final
	{
	public:
		//******************************
		// Ctor that takes in a concept
		// of an erased IManager type
		//******************************
		inline IManager(ManagerConcept* manager) noexcept : m_manager(manager) { }

		//******************************
		// Register passthrough method
		// using the Manager interface
		//******************************
		inline void Register(const char* handle, void* function) const noexcept { m_manager->Register(handle, function); }

		//******************************
		// Unregister passthrough method
		// using the Manager interface
		//******************************
		inline void Unregister(const char* handle, void* function) const noexcept { m_manager->Unregister(handle, function); }

		//******************************
		// Plugin function getter method
		// Useful for using functions
		// from other plugins, allowing
		// for library plugins
		//******************************
		template<typename FuncType>
		inline std::vector<FuncType> PluginFunctions(const char* handle) const noexcept
		{
			std::vector<void*> _f = m_manager->PluginFunctions(handle);
			std::vector<FuncType> r = {};
			for (void* f : _f) r.push_back(static_cast<FuncType>(f));
			return r;
		}
	private:
		// shared pointer to the base class
		// to utilize polymorphism without
		// exposing the underlying type
		std::shared_ptr<ManagerConcept> m_manager;
	};
}

//**************************************
// Macro definitions for the built-in
// functions required for each plugin
//**************************************

// function prefix
#define PLUGIN_EXPORT	extern "C" __declspec(dllexport)

// version getter for the plugin manager
#define GET_VERSION()	PLUGIN_EXPORT float dll_version() { return VERSION; }

// clean this up so someone else may use VERSION later
#undef VERSION

// dll entry point, register handles here
#define DLL_MAIN(M)		PLUGIN_EXPORT void dll_register(Plugin::IManager M)

// dll exit point, unregsiter handles here
#define DLL_EXIT(M)		PLUGIN_EXPORT void dll_unregister(Plugin::IManager M)