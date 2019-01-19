#pragma once
#include "Renderer.h"

class Search {
	private:
		static short startX;
		static short startY;

		static short currentX;
		static short currentY;

		static short destX;
		static short destY;

		static void getNewPosition(short&, short&, const int);
		static float getCost(const short, const short, const short, const short);
	public:
		static void setupPoints(const bool);
		static void setupAStar();
};

