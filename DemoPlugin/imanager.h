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

// minimum version this is compatible with
#define MIN_VERSION 0.0
// maximum version this is compatible with
#define MAX_VERSION 0.0

namespace Plugin
{
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
		// Templated ctor for erasing
		// the type that conforms to
		// the specified affordances --
		// in this case just Register
		//******************************
		template<typename Type>
		inline IManager(Type& manager) noexcept : m_manager(new ManagerModel<Type>(manager)) { }

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
		//******************************
		// Abstract base class, used for
		// erasing the type contained in
		// the child class ManagerModel
		//******************************
		struct ManagerConcept
		{
			// IOC Register method
			virtual void Register(const char*, void*) const noexcept = 0;
			// IOC Unregister method
			virtual void Unregister(const char*, void*) const noexcept = 0;
			// IOC Function getter
			virtual std::vector<void*> PluginFunctions(const char* handle) const noexcept = 0;
		};

		//******************************
		// Derived class that knows what
		// type it is erasing and can
		// call functions by the
		// interface it exposes
		//******************************
		template<typename Type>
		struct ManagerModel final : ManagerConcept
		{
			//**************************
			// ctor grabs a reference to
			// the item passed in and
			// holds on to it for
			// dispatching later
			//**************************
			inline ManagerModel(Type& manager) noexcept : m_manager(manager) { }

			//**************************
			// Final passthrough Register
			// function, used for
			// extracting the final
			// behavior from the erased
			// type that it contains
			//**************************
			virtual inline void Register(const char* handle, void* function) const noexcept override
			{
				m_manager.Register(handle, function);
			}

			//**************************
			// Final passthrough Unregister
			// function, used for removing
			// the final behavior when this
			// plugin is unloaded
			//**************************
			virtual inline void Unregister(const char* handle, void* function) const noexcept override
			{
				m_manager.Unregister(handle, function);
			}

			//**************************
			// Final passthrough 
			// Function getter, used for
			// getting existing funcs
			// from other plugins that
			// are registered with
			// the plugin manager
			//**************************
			virtual std::vector<void*> PluginFunctions(const char* handle) const noexcept override
			{
				return m_manager.GetPluginFuncs(handle);
			}

			// the type to be erased
			Type& m_manager;
		};

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
#define GET_VERSION()	PLUGIN_EXPORT bool dll_version(float v) { return v >= MIN_VERSION && v <= MAX_VERSION; }

// dll entry point, register handles here
#define DLL_MAIN(M)		PLUGIN_EXPORT void dll_register(Plugin::IManager M)

// dll exit point, unregsiter handles here
#define DLL_EXIT(M)		PLUGIN_EXPORT void dll_unregister(Plugin::IManager M)