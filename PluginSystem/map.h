//**************************************
// map.h
//
// Draws a map in the terminal
//
// Author: Nathan Ikola
// nathan.ikola@gmail.com
//**************************************
#pragma once

#include <iostream>

#include "plugin_manager.h"

class Map
{
public:
	Map()
	{
		// add any extra characters to our pool of possible characters
		using FuncType = std::vector<char>(*)();
		std::vector<FuncType> chars = Plugin::PMgr::GetPluginFuncs<FuncType>("mapSymbols");
		for (auto f : chars)
			for(char c : f())
				m_symbols.push_back(c);
	}

	void Draw(int w, int h, int seed = -1)
	{
		// generate a seed if necessary
		if (seed == -1)
			seed = rand();

		// execute any plugins that affect the seed
		// this member function explicitly handles the pattern of
		// x = func(x), where func is defined as X func(X x)
		// if no plugins handle this, then it returns x unchanged
		seed = Plugin::PMgr::GetInstance().ExecutePlugins("seedGeneration", seed);

		// randomize that many times
		for (int i = 0; i < seed; ++i) (void)rand();

		// start to generate our map
		for (int y{ 0 }; y < h; ++y)
		{
			for (int x{ 0 }; x < w; ++x)
			{
				// put the random symbol in our map
				std::cout << m_symbols[rand() % m_symbols.size()];
			}
			std::cout << '\n';
		}
	}
private:
	std::vector<char> m_symbols = {' ', '^', '.'};
};