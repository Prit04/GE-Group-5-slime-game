#include "NumberRenderer.h"

const bool NumberRenderer::digitPatt[10][7][5] = {
	{ { 0,1,1,1,0 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 0,1,1,1,0 } }, //0
	{ { 0,0,1,0,0 },{ 0,1,1,0,0 },{ 0,0,1,0,0 },{ 0,0,1,0,0 },{ 0,0,1,0,0 },{ 0,0,1,0,0 },{ 0,1,1,1,0 } }, //1
	{ { 0,1,1,1,0 },{ 1,0,0,0,1 },{ 0,0,0,0,1 },{ 0,0,1,1,0 },{ 0,1,0,0,0 },{ 1,0,0,0,0 },{ 1,1,1,1,1 } }, //2
	{ { 1,1,1,1,1 },{ 0,0,0,1,0 },{ 0,0,1,0,0 },{ 0,0,0,1,0 },{ 0,0,0,0,1 },{ 1,0,0,0,1 },{ 0,1,1,1,0 } }, //3
	{ { 0,0,0,1,0 },{ 0,0,1,1,0 },{ 0,1,0,1,0 },{ 1,0,0,1,0 },{ 1,1,1,1,1 },{ 0,0,0,1,0 },{ 0,0,0,1,0 } }, //4
	{ { 1,1,1,1,1 },{ 1,0,0,0,0 },{ 1,0,0,0,0 },{ 0,1,1,1,0 },{ 0,0,0,0,1 },{ 0,0,0,0,1 },{ 1,1,1,1,0 } }, //5 
	{ { 0,1,1,1,0 },{ 1,0,0,0,0 },{ 1,0,0,0,0 },{ 1,1,1,1,0 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 0,1,1,1,0 } }, //6 
	{ { 1,1,1,1,1 },{ 0,0,0,0,1 },{ 0,0,0,1,0 },{ 0,0,1,0,0 },{ 0,1,0,0,0 },{ 0,1,0,0,0 },{ 0,1,0,0,0 } }, //7 
	{ { 0,1,1,1,0 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 0,1,1,1,0 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 0,1,1,1,0 } }, //8
	{ { 0,1,1,1,0 },{ 1,0,0,0,1 },{ 1,0,0,0,1 },{ 0,1,1,1,1 },{ 0,0,0,0,1 },{ 0,0,0,0,1 },{ 0,1,1,1,0 } } //9 
};

NumberRenderer::NumberRenderer(int s, unsigned char red, unsigned char green, unsigned char blue) {
	scale = s;
	r = red;
	g = green;
	b = blue;
}

void NumberRenderer::drawOneDigit(GamesEngineeringBase::Window& canvas, int digit, int x, int y) {
	for (int row = 0; row < 7; row++) {
		for (int col = 0; col < 5; col++) {
			if (digitPatt[digit][row][col]) {
				for (int dy = 0; dy < scale; dy++) {
					for (int dx = 0; dx < scale; dx++) {
						int px = x + col * scale + dx;
						int py = y + row * scale + dy;

						canvas.draw(px, py, r, g, b);
					}
				}
			}
		}
	}
}

void NumberRenderer::drawWholeNumber(GamesEngineeringBase::Window& canvas, int number, int x, int y) {
	if (number == 0) {
		drawOneDigit(canvas, 0, x, y);
		return;
	}

	int temp = number;
	int numDigits = 0;
	while (temp > 0) {
		numDigits++;
		temp /= 10;
	}

	int currentX = x + (numDigits - 1) * (6 * scale);

	int currentNumber = number;
	while (currentNumber > 0) {
		int lastDigit = currentNumber % 10;

		drawOneDigit(canvas, lastDigit, currentX, y);

		currentNumber /= 10;
		currentX -= (6 * scale);
	}
}