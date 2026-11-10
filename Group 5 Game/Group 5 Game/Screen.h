#pragma once

#include "GamesEngineeringBase.h"
#include "NumberRenderer.h"
#include "Slime.h"
#include "ProjectileManager.h"

#include "Collectible.h"

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

private:

	// Slime character used only in the main menu
	Slime menuSlime;

	bool menuSlimeLoaded = false;
	float bounceTimer = 0.0f;
	bool wasMouseDown = false;

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


	Collectible collectible;	// Crystal spawning system
	Slime slime;
	Vector2<float> camera{ 0.0f, 0.0f };
	Vector2<float> moveInput{ 0.0f, 0.0f };
	bool slimeLoaded = false;

	ProjectileManager projectiles;
	int lives = 3;
	float invincibleTimer = 0.0f;
	float deathTimer = 0.0f;

	// rough hit circle for the slime
	static constexpr float SlimeHitRadius = 14.0f;
	static constexpr float SlimeCentreOffsetY = 15.0f;

	void UpdateProjectiles(float deltaTime);

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

	
// Get the game score
	int GetScore() const { return score; }

	// Get the survival time
	float GetElapsedTime() const { return elapsedTime; }
};

class GameOverScreen : public Screen
{


private:
	NumberRenderer resultRenderer{ 4, 255, 255, 255 };
	int finalScore = 0;
	float finalTime = 0.0f;
	bool wasMouseDown = false;

public:
	GameOverScreen(GamesEngineeringBase::Window& _window, GameManager& _gameManager) : Screen(_window, _gameManager)
	{
	}

	void Init() override;
	void HandleInput() override;
	void Update(float deltaTime) override;
	void Draw(GamesEngineeringBase::Window& window) override;
	void Destroy() override;


// Store the final game results
	void SetResults(int score, float time)
	{
		finalScore = score;
		finalTime = time;
	}
};