//**************************************
// plugin_handle.h
//
// Holds the declaration for the plugin
// handle system, which allows plugins
// to subscribe to specific handles when
// they are initialized
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************
#pragma once

#include <assert.h>
#include <functional>
#include <vector>

namespace Plugin
{
	class PluginHandle final
	{
	public:
		//************************************
		// Default ctor is acceptable
		//************************************
		inline PluginHandle() noexcept {};

		//************************************
		// Overload ctor to accept function
		//************************************
		inline PluginHandle(void* func) { Attach(func); }

		//************************************
		// Move constructor is acceptable
		//************************************
		inline PluginHandle(PluginHandle&& rhs) noexcept
		{
			m_functions = std::move(rhs.m_functions);
		}

		//************************************
		// Move assignment is acceptable
		//************************************
		PluginHandle& operator=(PluginHandle&& rhs) noexcept
		{
			if(this != &rhs)
				m_functions = std::move(rhs.m_functions);
			return *this;
		}

		//************************************
		// Standard shallow copy operations
		//************************************
		PluginHandle(const PluginHandle& rhs) noexcept = default;
		PluginHandle& operator=(const PluginHandle& rhs) noexcept = default;

		//************************************
		// Default dtor is acceptable
		//************************************
		inline ~PluginHandle() noexcept { m_functions.clear(); }

		//************************************
		// Attach a function to this handle
		//************************************
		inline void Attach(void* func) noexcept
		{
			assert(func != nullptr);
			// check that we do not already have this function
			for (void* f : m_functions) assert(f != func);
			// add this function to the working list
			m_functions.push_back(func);
		}

		//************************************
		// Detach a function from this handle
		//************************************
		inline void Detach(void* func) noexcept
		{
			for (auto iter{ m_functions.begin() }; iter != m_functions.end(); ++iter)
				if ((*iter) == func)
				{
					m_functions.erase(iter);
					return;
				}
			// failed to find func in m_functions
			// this is a bug, should not detach a
			// function that was never attached
			assert(false);
		}

		//************************************
		// Return a vector of function
		// pointers, specified by FuncPtr
		//************************************
		template<typename FuncPtr>
		std::vector<FuncPtr> As() const noexcept
		{
			// ensure we are casting to a function pointer
			// since we have erased this type completely
			static_assert(std::is_pointer<FuncPtr>::value 
				&& std::is_function<std::remove_pointer<FuncPtr>::type>::value, 
				"PluginHandle::As<T> requires that T is a function pointer");
			std::vector<FuncPtr> r;
			for (void* f : m_functions) 
				r.push_back(static_cast<FuncPtr>(f));
			return r;
		}

		//******************************
		// Return the vector of void*
		// pointers
		//******************************
		inline std::vector<void*> Pointers() const noexcept { return m_functions; }
	private:
		std::vector<void*> m_functions = {};
	};
}