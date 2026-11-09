#pragma once
#include "GamesEngineeringBase.h"

class NumberRenderer
{
private:
	int scale;
	unsigned char r, g, b;
	static const bool digitPatt[10][7][5];

	void drawOneDigit(GamesEngineeringBase::Window&, int digit, int x, int y);

public:
	NumberRenderer(int s, unsigned char red, unsigned char green, unsigned char blue);

	void drawWholeNumber(GamesEngineeringBase::Window&, int number, int x, int y);
};

