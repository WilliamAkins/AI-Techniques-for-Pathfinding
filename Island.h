#pragma once
#include <ctime>
#include "Renderer.h"

class Island {
	private:
		void markMidpoint(std::string&);
		std::string getIslandData();
	public:
		Island();
		~Island();
};