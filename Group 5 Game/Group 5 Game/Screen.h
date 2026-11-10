#pragma once

#include "GamesEngineeringBase.h"
#include "NumberRenderer.h"
#include "Slime.h"

// Need gamemanager for some things here, can change implementation later
// but for now this stops a recursive header read
class GameManager;

// Main Class that all screens derive from
class Screen
{
public:
	Screen(GamesEngineeringBase::Window& _window, GameManager& _gameManager) : window(_window), gameManager(_gameManager)
	{
	}
	virtual ~Screen()
	{
	}

	virtual void Init()
	{
	}
	virtual void HandleInput() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(GamesEngineeringBase::Window& window) = 0;
	virtual void Destroy() = 0;

protected:
	GamesEngineeringBase::Window& window;
	GameManager& gameManager;
};

// Menu Screen
class MenuScreen : public Screen
{

public:
	MenuScreen(GamesEngineeringBase::Window& _window, GameManager& _gameManager) : Screen(_window, _gameManager)
	{
	}
	void Init() override;
	void HandleInput() override;
	void Update(float deltaTime) override;
	void Draw(GamesEngineeringBase::Window& window) override;
	void Destroy() override;
};

// Game Screen
class GameScreen : public Screen
{
private:
	NumberRenderer hudRenderer{ 4, 255, 255, 255 };
	int score = 0;
	float elapsedTime = 0.0f;

	Slime slime;
	Vector2<float> camera{ 0.0f, 0.0f };
	Vector2<float> moveInput{ 0.0f, 0.0f };
	bool slimeLoaded = false;

	// A small world, larger than the window, so the camera has room to follow
	static constexpr int WorldWidth = 1600;
	static constexpr int WorldHeight = 1200;

	void LoadSlime();
	void UpdateCamera();
	void DrawFloor(GamesEngineeringBase::Window& window) const;

public:
	GameScreen(GamesEngineeringBase::Window& _window, GameManager& _gameManager) : Screen(_window, _gameManager)
	{
	}
	void Init() override;
	void HandleInput() override;
	void Update(float deltaTime) override;
	void Draw(GamesEngineeringBase::Window& window) override;
	void Destroy() override;
};

class GameOverScreen : public Screen
{
public:
	GameOverScreen(GamesEngineeringBase::Window& _window, GameManager& _gameManager) : Screen(_window, _gameManager)
	{
	}

	void Init() override;
	void HandleInput() override;
	void Update(float deltaTime) override;
	void Draw(GamesEngineeringBase::Window& window) override;
	void Destroy() override;
};