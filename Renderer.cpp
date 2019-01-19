#include <math.h>
#include "Renderer.h"
#include "Search.h"
#include "console.h"

//vector<char> Renderer::grid;
std::string Renderer::grid;
short Renderer::gridHeight;
short Renderer::gridWidth;
vector<short> Renderer::midpointPos;

void Renderer::setupGrid(const short height, const short width) {
	system("COLOR 90");

	Renderer::gridHeight = height;
	Renderer::gridWidth = width;

	//fill the grid with the water icon
	Renderer::grid.insert(Renderer::grid.begin(), Renderer::gridHeight * Renderer::gridWidth, 'w');
}

void Renderer::updateGrid(const short y, const short x, const char data, const bool overrideSquare) {
	short position = x + Renderer::gridWidth * y;
	//check that the modification to the grid will be within range and is water
	if ((position < Renderer::grid.size() && Renderer::grid[position] == 'w') || overrideSquare == true)
		Renderer::grid[position] = data;
}

void Renderer::updateGrid(const short position, const char data, const bool overrideSquare) {
	//check that the modification to the grid will be within range and is water
	if ((position < Renderer::grid.size() && Renderer::grid[position] == 'w') || overrideSquare == true)
		Renderer::grid[position] = data;
}

void Renderer::setupPaths() {
	//find each midpoint from all the islands
	for (short i = 0; i < Renderer::grid.length(); i++)
		if (Renderer::grid[i] == 'c')
			Renderer::midpointPos.push_back(i);


	//loop through each island
	for (int i = 0; i < Renderer::midpointPos.size() - 1; i++) {
		//get the x and y of the first midpoint
		float x1 = Renderer::midpointPos[i] % Renderer::gridWidth;
		float y1 = Renderer::midpointPos[i] / Renderer::gridWidth;

		//get the x and y of the second midpoint
		float x2 = Renderer::midpointPos[i + 1] % Renderer::gridWidth;
		float y2 = Renderer::midpointPos[i + 1] / Renderer::gridWidth;

		//calculate the difference between the two points
		float dx = x2 - x1;
		float dy = y2 - y1;

		//calculate the number of squares between the two points
		short steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

		//calculate how much a square should move every iteration
		float xInc = dx = dx / (float)steps;
		float yInc = dy = dy / (float)steps;

		//loop through, adding the squares between the two points
		for (int i = 0; i <= steps; i++) {
			updateGrid(y1, x1, 'p', true);

			x1 += xInc;
			y1 += yInc;
		}
	}
}

void Renderer::drawSquare(const short position) {
	switch (Renderer::grid[position]) {
		case 'w': //water
			Console::setColour(Console::BLUE, Console::LIGHT_BLUE);
			std::cout << char(177);
			break;
		case 'g': //grass
			Console::setColour(Console::GREEN, Console::BLACK);
			std::cout << char(219);
			break;
		case 'm': //mountain
			Console::setColour(Console::GRAY, Console::BLACK);
			std::cout << char(219);
			break;
		case 'a': //marshland
			Console::setColour(Console::BLACK, Console::BLACK);
			std::cout << char(219);
			break;
		case 'p': //path
			Console::setColour(Console::YELLOW, Console::BLACK);
			std::cout << char(219);
			break;
		case 'c': //mid-point / centre
			std::cout << char(219);
			break;
		case 's': //start
			Console::setColour(Console::RED, Console::BLACK);
			std::cout << char(219);
			break;
		case 'x': //destination
			Console::setColour(Console::BLUE, Console::BLACK);
			std::cout << char(219);
			break;
		case 'h': //search path
			Console::setColour(Console::PURPLE, Console::BLACK);
			std::cout << char(219);
			break;
		case 'o': //optimal path
			Console::setColour(Console::RED, Console::BLACK);
			std::cout << char(219);
			break;
		default:
			break;
	}
}

void Renderer::draw() {
	//treat the 1D array as 2d, ensuring the correct blocks are placed in the right positions
	for (short y = 0; y < Renderer::gridHeight; y++) {
		for (short x = 0; x < Renderer::gridWidth; x++) {
			Console::setCursorPosition(y, x);
			drawSquare(x + Renderer::gridWidth * y);
		}
	}
}

void Renderer::draw(const short y, const short x) {
	Console::setCursorPosition(y, x);
	drawSquare(x + Renderer::gridWidth * y);
}

void Renderer::draw(const short position) {
	float y = position / Renderer::gridWidth;
	float x = position % Renderer::gridWidth;

	Console::setCursorPosition(y, x);
	drawSquare(position);
}

short Renderer::getGridHeight() {
	return Renderer::gridHeight;
}

short Renderer::getGridWidth() {
	return Renderer::gridWidth;
}

char Renderer::getGridChar(const short pos) {
	if (pos >= 0 && pos < Renderer::grid.size())
		return Renderer::grid[pos];
	else
		return ' ';
}

char Renderer::getGridChar(const short y, const short x) {
	short pos = x + Renderer::gridWidth * y;
	if (pos >= 0 && pos < Renderer::grid.size())
		return Renderer::grid[pos];
	else
		return ' ';
}

vector<short>& Renderer::getMidpoints() {
	return Renderer::midpointPos;
}