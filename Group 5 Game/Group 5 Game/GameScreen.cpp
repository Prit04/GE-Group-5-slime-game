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

}

// Update logic here
void GameScreen::Update(float deltaTime)
{
	elapsedTime += deltaTime;
}

// Draw calls here
void GameScreen::Draw(GamesEngineeringBase::Window& window)
{
	hudRenderer.drawWholeNumber(window, score, 20, 20);
	hudRenderer.drawTime(window, static_cast<int>(elapsedTime), 20, 60);
}

// Destroy pointers etc, any stuff that would cause a mem leak
void GameScreen::Destroy()
{

}