#pragma once

#include <iostream>
#include <string>
#include<vector>

using std::vector;

class Renderer {
	private:
		static std::string grid;
		static short gridHeight;
		static short gridWidth;
		static vector<short> midpointPos;

	public:
		static void setupGrid(const short, const short);
		static void updateGrid(const short, const short, const char, const bool overrideSquare = false);
		static void updateGrid(const short, const char, const bool overrideSquare = false);
		static void setupPaths();
		static void drawSquare(const short);
		static void draw();
		static void draw(const short, const short);
		static void draw(const short);
		static short getGridHeight();
		static short getGridWidth();
		static char getGridChar(const short);
		static char getGridChar(const short, const short);
		static vector<short>& getMidpoints();
};