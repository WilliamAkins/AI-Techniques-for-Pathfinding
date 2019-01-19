#include "Island.h"

void Island::markMidpoint(std::string& islandData) {
	islandData[floor(islandData.length() * 0.5)] = 'c';
}

std::string Island::getIslandData() {
	std::string islandTemplate[3];

	islandTemplate[0] = {
		"wwwwggggggggggggggwwww\n"
		"wwwggggggggggggggggwww\n"
		"wwggggggggggggggggggww\n"
		"wggggggggggggggggggggw\n"
		"gggggggggggggggggggggg\n"
		"wggggggggggggggggggggw\n"
		"wwggggggggggggggggggww\n"
		"wwwggggggggggggggggwww\n"
		"wwwwggggggggggggggwwww\n"
	};

	islandTemplate[1] = {
		"wwwwgggggggggggggggggwwww\n"
		"wwwgggggmmmmmmggggggggwww\n"
		"wwgggggmmmmmmmmggggggggww\n"
		"wgggggmmmmmmmmmmggggggggw\n"
		"gggggggggmmmmmmmmmmgggggg\n"
		"wgggggmmmmmmmmmmggggggggw\n"
		"wwggggggggmmmmmmmmgggggww\n"
		"wwwggggggggmmmmmmgggggwww\n"
		"wwwwgggggggmmmmmmggggwwww\n"
		"wwwwggggmmmmmmgggggggwwww\n"
		"wwwwgggmmmmmmmmggggggwwww\n"
		"wwwwggmmmmmmmmmmgggggwwww\n"
		"wwwwgggggggggggggggggwwww\n"
	};

	islandTemplate[2] = {
		"wwwwggggggggggaaaawwww\n"
		"wwwgggggggggggaaaaawww\n"
		"wwgggggggggggggaaaaaww\n"
		"waaaaaagggggggggggaaaw\n"
		"gggaaaaaagggggggggggaa\n"
		"wgggaaaaaaagggggggaaaw\n"
		"wwggggaaaaaaagggggaaww\n"
		"wwwggaaaaaagggggggawww\n"
		"wwwwgaaaagggggggggwwww\n"
		"wwwgggaagggggggggggwww\n"
		"wwggggggggggggggggggww\n"
		"wggggggggggggggggggggw\n"
	};

	//randomly return an island template
	return islandTemplate[rand() % 3];
}

Island::Island() {
	std::string islandData = getIslandData();

	//modify the island data to add its centre point
	markMidpoint(islandData);

	short xOrigin = rand() % Renderer::getGridWidth() - 10;
	short yOrigin = rand() % Renderer::getGridHeight();
	short curXPos = xOrigin;
	short curYPos = yOrigin;

	for (int i = 0; i < islandData.length(); i++) {
		if (islandData[i] == '\n') { //if current char is '\n' which means a new line
			//reset the cursor position
			curYPos++;
			curXPos = xOrigin;
			//skip the '\n' char, therefore not drawing it
			i++;
		}
		curXPos++;

		Renderer::updateGrid(curYPos, curXPos, islandData[i]);
	}
}

Island::~Island() {}