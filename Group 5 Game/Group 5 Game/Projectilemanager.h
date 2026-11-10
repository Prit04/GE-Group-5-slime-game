#pragma once

#include "GamesEngineeringBase.h"
#include "Vector.h"
#include "Projectile.h"

// spawns projectiles just outside the camera's view, aimed at a target (the slime), and updates, draws and despawns them.
class ProjectileManager
{
public:
	// call when the game screen starts. viewWidth/viewHeight = window size
	void Init(int _viewWidth, int _viewHeight);

	// targetPos = where new projectiles aim, camera = top-left of the view in world space
	void Update(float deltaTime, const Vector2<float>& targetPos, const Vector2<float>& camera);
	void Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera) const;

	// will return true if an active projectile overlaps the given circle, and despawns it
	bool CheckHit(const Vector2<float>& pos, float radius);

	// despawn everything
	void Clear();

private:
	static const int MaxProjectiles = 64;
	Projectile pool[MaxProjectiles];

	int viewWidth = 1024;
	int viewHeight = 768;

	// spawning settings, tweak these to change difficulty
	float spawnTimer = 0.0f;
	float spawnInterval = 1.0f;                // seconds between spawns at the start
	static constexpr float StartInterval = 1.0f;
	static constexpr float MinSpawnInterval = 0.2f;   // fastest it can get
	static constexpr float IntervalDecrease = 0.02f;  // how much faster after each spawn
	static constexpr float ProjectileSpeed = 250.0f;  // pixels per second
	static constexpr float ProjectileRadius = 8.0f;
	static constexpr float DespawnMargin = 150.0f;    // how far outside the view before removal

	void SpawnProjectile(const Vector2<float>& targetPos, const Vector2<float>& camera);
	float RandomRange(float min, float max) const;
};