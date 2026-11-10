#include <iostream>

#include "Screen.h"
#include "GameManager.h"

// Standard init for loading images etc
void MenuScreen::Init()
{
	std::cout << "Menu Screen Loaded" << std::endl;
}

// Any inputs needed, add here
void MenuScreen::HandleInput()
{
	if (window.keyPressed('Q'))
	{
		gameManager.GetDifficulty().SetDifficultyType(DifficultyType::Easy);
		gameManager.SetState(GameState::Game);
	}
	if (window.keyPressed('W'))
	{
		gameManager.GetDifficulty().SetDifficultyType(DifficultyType::Medium);
		gameManager.SetState(GameState::Game);
	}
	if (window.keyPressed('E'))
	{
		gameManager.GetDifficulty().SetDifficultyType(DifficultyType::Hard);
		gameManager.SetState(GameState::Game);
	}
}

// Update logic here
void MenuScreen::Update(float deltaTime)
{

}

// Draw calls here
void MenuScreen::Draw(GamesEngineeringBase::Window& window)
{

}

// Destroy pointers etc, any stuff that would cause a mem leak
void MenuScreen::Destroy()
{

}