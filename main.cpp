#include <iostream>
#include <ctime>
#include "console.h"
#include "Renderer.h"
#include "Island.h"
#include "Search.h"

int main() {
	const short height = 50, width = 100;

	//setup the random seed and run rand() once to prevent a bug with visual studio
	srand(static_cast<unsigned int>(time(0)));
	rand();

	Console::setWindowTitle("CS3S667 - Scenario 1 | William Akins - 15029476");
	Console::setTextBufferSize(height, width);

	Renderer::setupGrid(height, width);

	for (int i = 0; i < 24; i++)
		Island island;
	
	Renderer::setupPaths();

	Renderer::draw();

	Search::setupPoints(true); //setup the starting point
	Search::setupPoints(false); //setup the destination

	std::getchar();

	Search::setupAStar();

	std::getchar();
	return 0;
}