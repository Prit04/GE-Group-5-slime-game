#include "GameManager.h"

// Initialise the game
int GameManager::Init()
{
	// Create window
	window.create(1024, 768, "Slime Game");

	isRunning = true;

	// Change the screen to the main menu
	ChangeScreen();

	return 1;
}

// Handle universal input across screens
// Can currently quit the game at any time
// Also forceload scenes for now through 1 - 3
void GameManager::HandleInput()
{
	if (window.keyPressed(VK_ESCAPE))
	{
		isRunning = false;
	}
	if (window.keyPressed('1'))
	{
		currentState = GameState::Menu;
		ChangeScreen();
	}
	else if (window.keyPressed('2'))
	{
		currentState = GameState::Game;
		ChangeScreen();
	}
	else if (window.keyPressed('3'))
	{
		currentState = GameState::GameOver;
		ChangeScreen();
	}
	if (currentScreen != nullptr)
	{
		currentScreen->HandleInput();
	}
}

// Update deltatime and call current screen's update
void GameManager::Update()
{
	deltaTime = timer.dt();
	if (currentScreen != nullptr)
	{
		currentScreen->Update(deltaTime);
	}
}

// Call current screen's draw
void GameManager::Draw()
{
	if (currentScreen != nullptr)
	{
		currentScreen->Draw(window);
	}
}

// Destroy current screen pointer
void GameManager::Destroy()
{
	if (currentScreen)
	{
		currentScreen = nullptr;
	}
}

// Change the screen state
void GameManager::ChangeScreen()
{
	// call destroy function for the previous screen being used
	if (currentScreen != nullptr)
	{
		currentScreen->Destroy();
	}

	// Switch to the new game state
	switch (currentState)
	{
	case GameState::Menu:
		currentScreen = &menuScreen;
		break;
	case GameState::Game:
		currentScreen = &gameScreen;
		break;
	case GameState::GameOver:
		currentScreen = &gameOverScreen;
		break;
	}

	// Call the screen's init function to set up screen
	if (currentScreen != nullptr)
	{
		currentScreen->Init();
	}
}