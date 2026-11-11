#include <algorithm>
#include <iostream>

#include "Screen.h"
#include "GameManager.h"

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


// Start spawning crystals automatically when gameplay begins
	collectible.Start();

	lives = StartLives;
	invincibleTimer = 0.0f;
	deathTimer = 0.0f;
	projectiles.Init(window.getWidth(), window.getHeight());

	gameManager.GetScoreManager().SetScoreMultiplier(gameManager.GetDifficulty().GetDifficultyType());
	projectiles.SetIntervalDecrease(gameManager.GetDifficulty().GetDifficultyType());

	// Only set up the slime the first time we enter the screen, so switching
	// between screens does not teleport it back to the middle

// Load the slime sprite only once
	if (!slimeLoaded)
	{
		LoadSlime();
	}
	// Reset the slime whenever a new game starts
	slime.Spawn({ WorldWidth * 0.5f, WorldHeight * 0.5f });

	slime.Spawn({ WorldWidth * 0.5f, WorldHeight * 0.5f });
	gameManager.GetScoreManager().Reset();

	UpdateCamera();

	collectible.Start();
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
	/*if (window.keyPressed('G'))
	{
		collectible.Start();
	}*/


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

	if (window.keyPressed(VK_SHIFT))
	{
		if (slime.TryDash())
		{
			lives--;
		}
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
	gameManager.GetScoreManager().Update(deltaTime);

// Update random crystal spawning
	collectible.Update(
		deltaTime,
		slime.GetPosition(),
		WorldWidth,
		WorldHeight
	);
	slime.Update(deltaTime, moveInput);

	// Keep the slime inside the world so it never walks off the map
	Vector2<float> position = slime.GetPosition();
	position.x = std::clamp(position.x, 30.0f, static_cast<float>(WorldWidth) - 30.0f);
	position.y = std::clamp(position.y, 40.0f, static_cast<float>(WorldHeight) - 10.0f);
	slime.SetPosition(position);

	UpdateCamera();


// Spawn crystals near the slime
	collectible.Update(
		deltaTime,
		slime.GetPosition(),
		WorldWidth,
		WorldHeight
	);

	
	// Check whether the slime collects crystals
	//collectible.CheckPickup(slime.GetPosition());

	UpdateProjectiles(deltaTime);
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

	collectible.Draw(window, camera);// Draw crystals

	slime.Draw(window, camera);
	projectiles.Draw(window, camera);

	hudRenderer.drawWholeNumber(window, lives, 20, 20);
	hudRenderer.drawWholeNumber(window, gameManager.GetScoreManager().GetScore(), 20, 60);
	hudRenderer.drawTime(window, static_cast<int>(gameManager.GetScoreManager().GetTime()), 20, 100);

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
	collectible.Clear();
}

// moves projectiles, handles crystal pickups and hits, and links health to the slime's size
void GameScreen::UpdateProjectiles(float deltaTime)
{
	// the hitbox grows and shrinks with the sprite
	const float scale = slime.GetScale();
	const float hitRadius = SlimeHitRadius * scale;
	const Vector2<float> slimeCentre = slime.GetPosition() - Vector2<float>(0.0f, SlimeCentreOffsetY * scale);

	projectiles.Update(deltaTime, slimeCentre, camera);

	// once dead, wait for the death animation then go to game over
	if (slime.IsDead())
	{
		deathTimer += deltaTime;
		if (deathTimer >= 1.0f)
		{
			gameManager.SetState(GameState::GameOver);
		}
		return;
	}

	// collecting crystals heals the slime and makes it bigger
	// (checked before the invincibility return, so you can still collect while flashing)
	const int collected = collectible.CheckHit(slimeCentre, hitRadius, gameManager.GetScoreManager());
	if (collected > 0)
	{
		collectedTotal += collected;
		if (collectedTotal % 3 != 0)
		{
			return;
		}

		lives += collected;
		if (lives > MaxLives)
		{
			lives = MaxLives;
		}
		UpdateSlimeSize();
	}

	// short invincibility after a hit
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= deltaTime;
		return;
	}

	// Getting hit loses health and makes the slime smaller
	if (projectiles.CheckHit(slimeCentre, hitRadius))
	{
		lives--;
		invincibleTimer = Slime::HurtDuration;

		if (lives <= 0)
		{
			slime.Die();
		}
		else
		{
			slime.Hurt();
			UpdateSlimeSize();
		}
	}
}

// Size is based on health: 3 lives = normal, each extra life = 20% bigger, each lost life = 20% smaller
void GameScreen::UpdateSlimeSize()
{
	slime.SetTargetScale(1.0f + (lives - StartLives) * SizePerLife);
}