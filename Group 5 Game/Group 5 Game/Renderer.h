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