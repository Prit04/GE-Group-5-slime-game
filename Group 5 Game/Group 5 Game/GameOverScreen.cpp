#include <iostream>

#include "Screen.h"
#include "GameManager.h"

// Standard init for loading images etc
void GameOverScreen::Init()
{
	ScoreManager scoreManager = gameManager.GetScoreManager();
	finalScore = scoreManager.GetScore();
	timeAlive = scoreManager.GetTime();

	// call once setting all stuff needed to display in gameover
	scoreManager.Reset();
	std::cout << "Game Over Screen Loaded" << std::endl;
}

// Any inputs needed, add here
void GameOverScreen::HandleInput()
{

}

// Update logic here
void GameOverScreen::Update(float deltaTime)
{

}

// Draw calls here
void GameOverScreen::Draw(GamesEngineeringBase::Window& window)
{

}

// Destroy pointers etc, any stuff that would cause a mem leak
void GameOverScreen::Destroy()
{

}