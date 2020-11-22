//**************************************
// dllmain.h
//
// Template for a plugin DLL main file
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************
#pragma once

#include "imanager.h"

// default implementation of GET_VERSION() returns VERSION
GET_VERSION()

// required to have the plugin entry point
// for registering plugin functions
// manager is the name of the IManager class
// that you need to Register your
// handlers with, and any initialization
// code required by your plugin
DLL_MAIN(manager);

// required to have the plugin exit point
// for unregistering plugin functions
// manager is the name of the IManager class
// that you need to Unregister your
// handlers from, and any cleanup code
// required by your plugin
DLL_EXIT(manager);