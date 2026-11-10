#include <iostream>

#include "Screen.h"

// Standard init for loading images etc
void GameScreen::Init()
{
	std::cout << "Game Screen Loaded" << std::endl;
	score = 0;
	elapsedTime = 0.0f;
}

// Any inputs needed, add here
void GameScreen::HandleInput()
{


// Press G to start automatic crystal spawning
	if (window.keyPressed('G'))
	{
		collectible.Start();
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
}

// Draw calls here
void GameScreen::Draw(GamesEngineeringBase::Window& window)
{

	collectible.Draw(window);// Draw crystals
	
	hudRenderer.drawWholeNumber(window, score, 20, 20);
	hudRenderer.drawTime(window, static_cast<int>(elapsedTime), 20, 60);

}

// Destroy pointers etc, any stuff that would cause a mem leak
void GameScreen::Destroy()
{

}
