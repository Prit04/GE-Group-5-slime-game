#include <cstdlib>
#include <ctime>

#include "ProjectileManager.h"

void ProjectileManager::Init(int _viewWidth, int _viewHeight)
{
	viewWidth = _viewWidth;
	viewHeight = _viewHeight;
	spawnTimer = 0.0f;
	spawnInterval = StartInterval;

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	Clear();
}

void ProjectileManager::Update(float deltaTime, const Vector2<float>& targetPos, const Vector2<float>& camera)
{
	// spawn a new projectile when the timer runs out, and get slightly faster each time
	spawnTimer += deltaTime;
	if (spawnTimer >= spawnInterval)
	{
		spawnTimer = 0.0f;
		SpawnProjectile(targetPos, camera);

		spawnInterval -= IntervalDecrease;
		if (spawnInterval < MinSpawnInterval)
		{
			spawnInterval = MinSpawnInterval;
		}
	}

	// the area projectiles are allowed in: the camera view plus a margin
	const float left = camera.x - DespawnMargin;
	const float top = camera.y - DespawnMargin;
	const float right = camera.x + viewWidth + DespawnMargin;
	const float bottom = camera.y + viewHeight + DespawnMargin;

	// move active projectiles and despawn any that have left that area
	for (int i = 0; i < MaxProjectiles; i++)
	{
		if (!pool[i].IsActive())
		{
			continue;
		}

		pool[i].Update(deltaTime);

		if (pool[i].IsOutside(left, top, right, bottom))
		{
			pool[i].Deactivate();
		}
	}
}

void ProjectileManager::Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera) const
{
	for (int i = 0; i < MaxProjectiles; i++)
	{
		if (pool[i].IsActive())
		{
			pool[i].Draw(window, camera);
		}
	}
}

bool ProjectileManager::CheckHit(const Vector2<float>& pos, float radius)
{
	for (int i = 0; i < MaxProjectiles; i++)
	{
		if (pool[i].IsActive() && pool[i].CollidesWith(pos, radius))
		{
			pool[i].Deactivate();
			return true;
		}
	}
	return false;
}

void ProjectileManager::Clear()
{
	for (int i = 0; i < MaxProjectiles; i++)
	{
		pool[i].Deactivate();
	}
}

// Pick a random edge of the camera view, spawn just outside it, and aim at the target's current position
void ProjectileManager::SpawnProjectile(const Vector2<float>& targetPos, const Vector2<float>& camera)
{
	// find a free slot in the pool, skip spawning if it's full
	Projectile* slot = nullptr;
	for (int i = 0; i < MaxProjectiles; i++)
	{
		if (!pool[i].IsActive())
		{
			slot = &pool[i];
			break;
		}
	}
	if (slot == nullptr)
	{
		return;
	}

	const float offset = ProjectileRadius * 2.0f;
	const float left = camera.x;
	const float top = camera.y;
	const float right = camera.x + viewWidth;
	const float bottom = camera.y + viewHeight;

	Vector2<float> spawnPos;
	switch (std::rand() % 4)
	{
	case 0: // left
		spawnPos.Set(left - offset, RandomRange(top, bottom));
		break;
	case 1: // right
		spawnPos.Set(right + offset, RandomRange(top, bottom));
		break;
	case 2: // top
		spawnPos.Set(RandomRange(left, right), top - offset);
		break;
	default: // bottom
		spawnPos.Set(RandomRange(left, right), bottom + offset);
		break;
	}

	const Vector2<float> direction = (targetPos - spawnPos).Normalised();
	slot->Activate(spawnPos, direction * ProjectileSpeed, ProjectileRadius);
}

float ProjectileManager::RandomRange(float min, float max) const
{
	const float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	return min + t * (max - min);
}