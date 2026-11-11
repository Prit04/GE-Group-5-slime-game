#pragma once

#include <iostream>

#include "Vector.h"

inline void DrawImage(GamesEngineeringBase::Window& window, const GamesEngineeringBase::Image& image, int x, int y)
{
	// Return if insufficient data is found for the image
	if (image.data == nullptr)
	{
		return;
	}

	// Loop through the height and width in pixels of the image
	for (unsigned int h = 0; h < image.height; h++)
	{
		// Calculate screent position of the current pixel and ensure it is on screen
		const int screenY = y + static_cast<int>(h);

		// Skip pixels outside of the screen
		if (screenY < 0 || screenY >= window.getHeight())
		{
			continue;
		}

		for (unsigned int w = 0; w < image.width; w++)
		{
			// Calculate screent position of the current pixel and ensure it is on screen
			const int screenX = x + static_cast<int>(w);

			// Skip pixels outside the screen
			if (screenX < 0 || screenX >= window.getWidth())
			{
				continue;
			}

			// Get the index (taking into account width and amount of channels (rgb, rgba)) at the position of the pixel in the data
			const unsigned int pixelIndex = (h * image.width + w) * image.channels;

			// Only draw pixels without an alpha value
			if (image.channels == 4 && image.data[pixelIndex + 3] != 0)
			{
				window.draw(screenX, screenY, &image.data[pixelIndex]);
			}
		}
	}
}

// Draws one rectangular region of an image at (x, y).
// DrawImage can only draw a whole image, so this is the small addition we need
// to animate a character that lives in one frame of a sprite sheet.
inline void DrawImageRegion(GamesEngineeringBase::Window& window,
	const GamesEngineeringBase::Image& image,
	int sourceX, int sourceY, int sourceWidth, int sourceHeight,
	int x, int y)
{
	// Return if insufficient data is found for the image
	if (image.data == nullptr)
	{
		return;
	}

	for (int h = 0; h < sourceHeight; h++)
	{
		// Position of the pixel on screen, skipping anything off screen
		const int screenY = y + h;

		if (screenY < 0 || screenY >= static_cast<int>(window.getHeight()))
		{
			continue;
		}

		const int sourceYRow = sourceY + h;

		if (sourceYRow < 0 || sourceYRow >= static_cast<int>(image.height))
		{
			continue;
		}

		for (int w = 0; w < sourceWidth; w++)
		{
			const int screenX = x + w;

			if (screenX < 0 || screenX >= static_cast<int>(window.getWidth()))
			{
				continue;
			}

			const int sourceXColumn = sourceX + w;

			if (sourceXColumn < 0 || sourceXColumn >= static_cast<int>(image.width))
			{
				continue;
			}

			// Index of the pixel inside the full sheet
			const unsigned int pixelIndex =
				(sourceYRow * image.width + sourceXColumn) * image.channels;

			// Only draw pixels without an alpha value
			if (image.channels == 4 && image.data[pixelIndex + 3] != 0)
			{
				window.draw(screenX, screenY, &image.data[pixelIndex]);
			}
		}
	}
}

// same as DrawImageRegion, but stretched by scale (1.0 = normal size).
// each screen pixel looks up the matching pixel in the sheet (nearest neighbour), so pixel art stays sharp.
inline void DrawImageRegionScaled(GamesEngineeringBase::Window& window,
	const GamesEngineeringBase::Image& image,
	int sourceX, int sourceY, int sourceWidth, int sourceHeight,
	int x, int y, float scale)
{
	if (image.data == nullptr || scale <= 0.0f)
	{
		return;
	}

	const int destWidth = static_cast<int>(sourceWidth * scale);
	const int destHeight = static_cast<int>(sourceHeight * scale);

	for (int dy = 0; dy < destHeight; dy++)
	{
		const int screenY = y + dy;
		if (screenY < 0 || screenY >= static_cast<int>(window.getHeight()))
		{
			continue;
		}

		const int sourceYRow = sourceY + static_cast<int>(dy / scale);
		if (sourceYRow < 0 || sourceYRow >= static_cast<int>(image.height))
		{
			continue;
		}

		for (int dx = 0; dx < destWidth; dx++)
		{
			const int screenX = x + dx;
			if (screenX < 0 || screenX >= static_cast<int>(window.getWidth()))
			{
				continue;
			}

			const int sourceXColumn = sourceX + static_cast<int>(dx / scale);
			if (sourceXColumn < 0 || sourceXColumn >= static_cast<int>(image.width))
			{
				continue;
			}

			const unsigned int pixelIndex =
				(sourceYRow * image.width + sourceXColumn) * image.channels;

			if (image.channels == 4 && image.data[pixelIndex + 3] != 0)
			{
				window.draw(screenX, screenY, &image.data[pixelIndex]);
			}
		}
	}
}
