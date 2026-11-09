#pragma once
#include "GamesEngineeringBase.h"

class NumberRenderer
{
private:
	int scale;
	unsigned char r, g, b;
	static const bool digitPatt[10][7][5];

	void drawBlock(GamesEngineeringBase::Window& canvas, int x, int y);
	void drawOneDigit(GamesEngineeringBase::Window& canvas, int digit, int x, int y);
	void drawColon(GamesEngineeringBase::Window& canvas, int x, int y);

public:
	NumberRenderer(int s, unsigned char red, unsigned char green, unsigned char blue);

	void drawWholeNumber(GamesEngineeringBase::Window&, int number, int x, int y);

	void drawTime(GamesEngineeringBase::Window& canvas, int totalSeconds, int x, int y);
};