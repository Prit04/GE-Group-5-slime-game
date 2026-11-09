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

// draws a scale x scale square at (x, y), skipping anything off-screen
void NumberRenderer::drawBlock(GamesEngineeringBase::Window& canvas, int x, int y)
{
	int w = static_cast<int>(canvas.getWidth());
	int h = static_cast<int>(canvas.getHeight());
	for (int dy = 0; dy < scale; dy++) {
		for (int dx = 0; dx < scale; dx++) {
			int px = x + dx;
			int py = y + dy;
			if (px >= 0 && py >= 0 && px < w && py < h) {
				canvas.draw(px, py, r, g, b);
			}
		}
	}
}

void NumberRenderer::drawOneDigit(GamesEngineeringBase::Window& canvas, int digit, int x, int y)
{
	for (int row = 0; row < 7; row++) {
		for (int col = 0; col < 5; col++) {
			if (digitPatt[digit][row][col]) {
				drawBlock(canvas, x + col * scale, y + row * scale);
			}
		}
	}
}

void NumberRenderer::drawColon(GamesEngineeringBase::Window& canvas, int x, int y)
{
	drawBlock(canvas, x + scale, y + 2 * scale);
	drawBlock(canvas, x + scale, y + 4 * scale);
}

void NumberRenderer::drawWholeNumber(GamesEngineeringBase::Window& canvas, int number, int x, int y)
{
	if (number < 0) number = 0; // only used for score/time, never negative

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

	// draw from right to left, starting from the last digit
	int currentX = x + (numDigits - 1) * (6 * scale);
	int currentNumber = number;
	while (currentNumber > 0) {
		drawOneDigit(canvas, currentNumber % 10, currentX, y);
		currentNumber /= 10;
		currentX -= (6 * scale);
	}
}

void NumberRenderer::drawTime(GamesEngineeringBase::Window& canvas, int totalSeconds, int x, int y)
{
	if (totalSeconds < 0) totalSeconds = 0;
	int minutes = totalSeconds / 60;
	int seconds = totalSeconds % 60;

	// mins
	drawWholeNumber(canvas, minutes, x, y);
	int minuteDigits = 1;
	for (int t = minutes; t >= 10; t /= 10) minuteDigits++;
	int cx = x + minuteDigits * 6 * scale;

	// colon 
	drawColon(canvas, cx, y);
	cx += 3 * scale;

	// secs
	drawOneDigit(canvas, seconds / 10, cx, y);
	drawOneDigit(canvas, seconds % 10, cx + 6 * scale, y);
}