#include <algorithm>
#include <iostream>

#include "Screen.h"

// The sprite sheet lives in the shared Assets folder, which is one level above
// the project folder. A few candidates are tried so the game also runs when it
// is started from a different working directory.
static const char* SlimeSheetPaths[] =
{
	"..\\Assets\\slime-spritesheet-50x50.png",
	"Assets\\slime-spritesheet-50x50.png",
	"..\\..\\Assets\\slime-spritesheet-50x50.png"
};

// Standard init for loading images etc
void GameScreen::Init()
{
	std::cout << "Game Screen Loaded" << std::endl;

	score = 0;
	elapsedTime = 0.0f;

	// Only set up the slime the first time we enter the screen, so switching
	// between screens does not teleport it back to the middle
	if (!slimeLoaded)
	{
		LoadSlime();
		slime.Spawn({ WorldWidth * 0.5f, WorldHeight * 0.5f });
	}

	UpdateCamera();
}

// Loads the slime sheet once and reports which path worked
void GameScreen::LoadSlime()
{
	if (slimeLoaded)
	{
		return;
	}

	for (const char* path : SlimeSheetPaths)
	{
		if (slime.Load(path))
		{
			std::cout << "Loaded slime sheet: " << path << std::endl;
			slimeLoaded = true;
			return;
		}
	}

	std::cout << "Failed to load the slime sheet" << std::endl;
}

// Any inputs needed, add here
// WASD moves, H takes a hit, K kills the slime, R resets it
void GameScreen::HandleInput()
{
	moveInput.Set(0.0f, 0.0f);


// Press G to start automatic crystal spawning
	if (window.keyPressed('G'))
	{
		collectible.Start();
	}


	if (window.keyPressed('W') || window.keyPressed(VK_UP))
	{
		moveInput.y -= 1.0f;
	}
	if (window.keyPressed('S') || window.keyPressed(VK_DOWN))
	{
		moveInput.y += 1.0f;
	}
	if (window.keyPressed('A') || window.keyPressed(VK_LEFT))
	{
		moveInput.x -= 1.0f;
	}
	if (window.keyPressed('D') || window.keyPressed(VK_RIGHT))
	{
		moveInput.x += 1.0f;
	}

	// These are edge triggered, so one press gives one animation
	if (window.keyPressed('H'))
	{
		slime.Hurt();
	}
	if (window.keyPressed('K'))
	{
		slime.Die();
	}
	if (window.keyPressed('R'))
	{
		slime.Reset();
	}
}

// Update logic here
void GameScreen::Update(float deltaTime)
{
	elapsedTime += deltaTime;


// Update random crystal spawning
	collectible.Update(
		deltaTime,
		window.getWidth(),
		window.getHeight()
	);
	slime.Update(deltaTime, moveInput);

	// Keep the slime inside the world so it never walks off the map
	Vector2<float> position = slime.GetPosition();
	position.x = std::clamp(position.x, 30.0f, static_cast<float>(WorldWidth) - 30.0f);
	position.y = std::clamp(position.y, 40.0f, static_cast<float>(WorldHeight) - 10.0f);
	slime.SetPosition(position);

	UpdateCamera();
}

// Camera follows the slime and stops at the edges of the world
void GameScreen::UpdateCamera()
{
	const float halfWidth = static_cast<float>(window.getWidth()) * 0.5f;
	const float halfHeight = static_cast<float>(window.getHeight()) * 0.5f;

	const float maxX = static_cast<float>(WorldWidth) - static_cast<float>(window.getWidth());
	const float maxY = static_cast<float>(WorldHeight) - static_cast<float>(window.getHeight());

	camera.x = std::clamp(slime.GetPosition().x - halfWidth, 0.0f, maxX);
	camera.y = std::clamp(slime.GetPosition().y - halfHeight, 0.0f, maxY);
}

// Draw calls here
void GameScreen::Draw(GamesEngineeringBase::Window& window)
{
	
	DrawFloor(window);
	slime.Draw(window, camera);

	collectible.Draw(window, camera);// Draw crystals

	hudRenderer.drawWholeNumber(window, score, 20, 20);
	hudRenderer.drawTime(window, static_cast<int>(elapsedTime), 20, 60);

}

// A cheap grid so the camera movement is visible. It is drawn in world space,
// so the lines scroll when the camera moves.
void GameScreen::DrawFloor(GamesEngineeringBase::Window& window) const
{
	const int gridSize = 100;
	const int width = static_cast<int>(window.getWidth());
	const int height = static_cast<int>(window.getHeight());

	const int offsetX = static_cast<int>(camera.x) % gridSize;
	const int offsetY = static_cast<int>(camera.y) % gridSize;

	// Vertical lines
	for (int x = gridSize - offsetX; x < width; x += gridSize)
	{
		for (int y = 0; y < height; y++)
		{
			window.draw(x, y, 45, 70, 55);
		}
	}

	// Horizontal lines
	for (int y = gridSize - offsetY; y < height; y += gridSize)
	{
		for (int x = 0; x < width; x++)
		{
			window.draw(x, y, 45, 70, 55);
		}
	}

	// World border, so it is clear where the camera stops.
	// These are the screen positions of the four world edges; a line is only
	// drawn when that edge is actually on screen.
	const int leftX = -static_cast<int>(camera.x);
	const int rightX = WorldWidth - static_cast<int>(camera.x);
	const int topY = -static_cast<int>(camera.y);
	const int bottomY = WorldHeight - static_cast<int>(camera.y);

	if (topY >= 0 && topY < height)
	{
		for (int x = 0; x < width; x++)
		{
			window.draw(x, topY, 200, 170, 90);
		}
	}

	if (bottomY >= 0 && bottomY < height)
	{
		for (int x = 0; x < width; x++)
		{
			window.draw(x, bottomY, 200, 170, 90);
		}
	}

	if (leftX >= 0 && leftX < width)
	{
		for (int y = 0; y < height; y++)
		{
			window.draw(leftX, y, 200, 170, 90);
		}
	}

	if (rightX >= 0 && rightX < width)
	{
		for (int y = 0; y < height; y++)
		{
			window.draw(rightX, y, 200, 170, 90);
		}
	}
}

// Destroy pointers etc, any stuff that would cause a mem leak
void GameScreen::Destroy()
{

}
