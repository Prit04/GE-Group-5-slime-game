#pragma once

#include "GamesEngineeringBase.h"
#include "Screen.h"
#include "Renderer.h"
#include "ScoreManager.h"

// Enum for game screens
enum class GameState
{
	Menu,
	Game,
	GameOver
};

class GameManager
{
public:
	GameManager()
	{
		currentState = GameState::Menu;
	}

	int Init();
	void HandleInput();
	void Update();
	void Draw();
	void Destroy();

	// Used for main.cpp
	bool IsRunning() const
	{
		return isRunning;
	}

	// !! If you want to change state to another scene (ie: player dead -> game over screen) CALL THIS FUNCTION !!
	// It'll handle switching for you, just set what enum state you need
	void SetState(GameState state)
	{
		currentState = state;
		ChangeScreen();
	}
	GameState GetState() const
	{
		return currentState;
	}
	GamesEngineeringBase::Window& GetWindow()
	{
		return window;
	}
	ScoreManager& GetScoreManager()
	{
		return scoreManager;
	}
private:
	GamesEngineeringBase::Window window;
	GamesEngineeringBase::Timer timer;

	// Pointers for game screen
	GameState currentState;
	Screen* currentScreen = nullptr;

	MenuScreen menuScreen{ window, *this };
	GameScreen gameScreen{ window, *this };
	GameOverScreen gameOverScreen{ window, *this };

	ScoreManager scoreManager;

	bool isRunning = false;
	float deltaTime = 0.0f;

	void ChangeScreen();
};

