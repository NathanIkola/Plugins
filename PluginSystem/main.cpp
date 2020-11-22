//**************************************
// main.cpp
//
// Driver program for testing my
// plugin system
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************

#include <functional>
#include <iostream>

#include "map.h"
#include "plugin_manager.h"

using std::cout;
const char* endl = "\n";

using namespace Plugin;

int main()
{
	// load our plugin (comment out to not load it)
	PMgr::GetInstance().LoadPlugin("DemoPlugin.dll");

	Map map;
	map.Draw(10, 10);

	return 0;
}