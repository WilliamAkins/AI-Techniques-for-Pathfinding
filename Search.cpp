#include <algorithm>
#include <limits.h>
#include <math.h>
#include "console.h"
#include "Search.h"

//forward reference the member variables
short Search::startX;
short Search::startY;
short Search::currentX;
short Search::currentY;
short Search::destX;
short Search::destY;

void Search::setupPoints(const bool isStart) {
	short randPos;

	//ensure the destination point is not placed on water or mountains
	char existingSquare = 'w';
	while (existingSquare == 'w' || existingSquare == 'm') {
		randPos = rand() % (Renderer::getGridWidth() * Renderer::getGridHeight());
		existingSquare = Renderer::getGridChar(randPos);
	}

	short y = randPos / Renderer::getGridWidth();
	short x = randPos % Renderer::getGridWidth();

	//change the symbol to be placed on the grid depending on params
	char symbol;
	if (isStart) {
		symbol = 's';

		Search::startY = y;
		Search::startX = x;

		Search::currentY = y;
		Search::currentX = x;
	} else {
		symbol = 'x';

		Search::destY = y;
		Search::destX = x;
	}

	Renderer::updateGrid(y, x, symbol, true);
	Renderer::draw(y, x);
}

void Search::getNewPosition(short& y, short& x, const int dir) {
	//modify the current position based off the chosen direction
	switch (dir) {
		case 0: //North
			y--;
			return;
		case 1: //East
			x++;
			return;
		case 2: //South
			y++;
			return;
		case 3: //West
			x--;
			return;
		case 4: //North East
			y--;
			x++;
			return;
		case 5: //South East
			y++;
			x++;
			return;
		case 6: //South West
			y++;
			x--;
			return;
		case 7: //North West
			y++;
			x--;
			return;
	}
}
static bool done = false;
float Search::getCost(const short y, const short x, const short y2, const short x2) {
	short position = x + Renderer::getGridWidth() * y;
	//get the weight of the new squre
	float weight;
	switch (Renderer::getGridChar(position)) {
		case 'p':
			weight = 1;
			break;
		case 'g':
			weight = 2;
			break;
		case 'a':
			weight = 3;
			break;
		default: //the default treats the cost the same as grass
			weight = 2;
			break;
	}

	float dy = abs(y - y2);
	float dx = abs(x - x2);

	float distance = dx > dy ? dx : dy; //Chebyshev Distance

	return distance + weight;
}

void Search::setupAStar() {
	//store the data about each square in a struct
	struct Squares {
		short y;
		short x;
		short parentY;
		short parentX;
		float gCost; //cost from the start
		float hCost; //cost to destination
	};
	std::vector<Squares> queue; //Open list | stores the array positions and cost of the different squares
	std::vector<Squares> closedList; //Squares that have already been evaluated

	//add the starting square to the open list / queue
	queue.push_back({ Search::currentY, Search::currentX, 0 });

	//add the starting node to the queue
	float gcost = getCost(Search::currentY, Search::currentX, Search::startY, Search::startX);
	float hcost = gcost + getCost(Search::currentY, Search::currentX, Search::destY, Search::destX);
	queue.push_back({ Search::currentY, Search::currentX, Search::currentY, Search::currentX, gcost, hcost });

	do {
		Search::currentY = queue.front().y;
		Search::currentX = queue.front().x;

		//update the grid array and re-render the chosen square
		Renderer::updateGrid(queue.front().y, queue.front().x, 'h', true);
		Renderer::draw(queue.front().y, queue.front().x);

		for (int i = 0; i < 8; i++) { //in order N-E-S-W-NE-SE-SW-NW | 0-7 inclusive
			short tmpY = Search::currentY;
			short tmpX = Search::currentX;
			//y + x passed by pointer, updating the position of the tmp X+Y
			getNewPosition(tmpY, tmpX, i);

			//iterate through the closed list, checking if the square is already in it
			auto it = std::find_if(closedList.begin(), closedList.end(), [&tmpY, &tmpX](const Squares& tmpSquare) {
				return tmpSquare.y == tmpY && tmpSquare.x == tmpX;
			});

			//verify whether the square should be placed, if it should then add it to the back of the queue
			if (it == closedList.end()) { // ensure that the square isn't already on the closed list
				if (tmpY >= 0 && tmpY < Renderer::getGridHeight() && tmpX >= 0 && tmpX < Renderer::getGridWidth()) {
					char squareType = Renderer::getGridChar(tmpY, tmpX);
					if (squareType != 'w' && squareType != 'm') {

						//check whether the square is already in the open list / queue
						auto it = std::find_if(queue.begin(), queue.end(), [&tmpY, &tmpX](const Squares& tmpSquare) {
							return tmpSquare.y == tmpY && tmpSquare.x == tmpX;
						});

						//get the new costs of the square
						float newGCost = getCost(tmpY, tmpX, Search::startY, Search::startX);
						float newHCost = getCost(tmpY, tmpX, Search::destY, Search::destX);
						float movementCost = queue.front().gCost + getCost(Search::currentY, Search::currentX, tmpY, tmpX) + newHCost;

						if (movementCost < newGCost || it == queue.end()) { //if square is not in the open list
							if (it == queue.end()) {
								queue.push_back({ tmpY, tmpX, Search::currentY, Search::currentX, movementCost, newHCost });
							} else {
								short index = std::distance(queue.begin(), it);
								queue[index].gCost = movementCost;
								queue[index].parentY = Search::currentY;
								queue[index].parentX = Search::currentX;
							}
						}
					}
				}
			}
		}

		//sort the queue from cheapest to most expensive
		std::sort(queue.begin(), queue.end(), [](Squares lhs, Squares rhs) {
			return lhs.gCost + lhs.hCost < rhs.gCost + rhs.hCost;
		});

		closedList.push_back(queue.front());
		queue.erase(queue.begin());
	}
	while (!queue.empty() && !(Search::currentX == Search::destX && Search::currentY == Search::destY));

	std::getchar();

	//only find the optimal path if the search has reached the destination
	if (Search::currentX == Search::destX && Search::currentY == Search::destY) {
		//add the destination to the closed list
		closedList.push_back(queue.front());

		short currentY = Search::currentY, currentX = Search::currentX;
		while (!(currentY == Search::startY && currentX == Search::startX)) {
			//update the grid and re-draw the display
			Renderer::updateGrid(currentY, currentX, 'o', true);
			Renderer::draw(currentY, currentX);

			//iterate through the closed list, looking for the square based off of its X and Y positions
			auto it = std::find_if(closedList.begin(), closedList.end(), [&currentY, &currentX](const Squares& tmpSquare) {
				return tmpSquare.y == currentY && tmpSquare.x == currentX;
			});

			//update the current X and Y with the parent position
			short index = std::distance(closedList.begin(), it);
			currentY = closedList[index].parentY;
			currentX = closedList[index].parentX;
		}

		//finally update the grid and draw the square with the starting square
		Renderer::updateGrid(currentY, currentX, 'o', true);
		Renderer::draw(currentY, currentX);

		Console::setColour(Console::PURPLE, Console::BLACK);
		Console::setCursorPosition(0, 0);
		std::cout << "Optimal path found.";
	} else {
		Console::setColour(Console::PURPLE, Console::BLACK);
		Console::setCursorPosition(0, 0);
		std::cout << "No path attainable.";
	}

}