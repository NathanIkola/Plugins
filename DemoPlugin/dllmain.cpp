//**************************************
// dllmain.cpp
//
// Holds the main implementation for
// my plugin DLL
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************
#include "dllmain.h"

#include <vector>

//**************************************
// The functions that the plugin will
// hand to the app to use
//**************************************

// extra characters
std::vector<char> chars()
{
	return { '#', '-' };
}

// seed manipulation function
int edit_seed(int seed)
{
	// idk maybe 0 is a really good seed
	return 0;
}

//**************************************
// The minimum function requirements
// that every plugin needs
//**************************************

// define our register method
DLL_MAIN(manager)
{
	manager.Register("mapSymbols", chars);
	manager.Register("seedGeneration", edit_seed);
}

// define our unregister method
DLL_EXIT(manager)
{
	manager.Unregister("mapSymbols", chars);
	manager.Unregister("seedGeneration", edit_seed);
}