#include "Projectile.h"

void Projectile::Activate(const Vector2<float>& _position, const Vector2<float>& _velocity, float _radius)
{
	position = _position;
	velocity = _velocity;
	radius = _radius;
	active = true;
}

void Projectile::Deactivate()
{
	active = false;
}

bool Projectile::IsActive() const
{
	return active;
}

void Projectile::Update(float deltaTime)
{
	position = position + velocity * deltaTime;
}

// draw it as a filled circle, skipping pixels outside the window
// (window.draw() has no bounds checks, and projectiles spawn off screen)
void Projectile::Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera) const
{
	const int w = static_cast<int>(window.getWidth());
	const int h = static_cast<int>(window.getHeight());
	const int r = static_cast<int>(radius);

	// the world position to screen position
	const int cx = static_cast<int>(position.x - camera.x);
	const int cy = static_cast<int>(position.y - camera.y);

	for (int dy = -r; dy <= r; dy++)
	{
		for (int dx = -r; dx <= r; dx++)
		{
			if (dx * dx + dy * dy <= r * r)
			{
				const int px = cx + dx;
				const int py = cy + dy;
				if (px >= 0 && py >= 0 && px < w && py < h)
				{
					window.draw(px, py, 255, 60, 60);
				}
			}
		}
	}
}

bool Projectile::IsOutside(float left, float top, float right, float bottom) const
{
	return position.x < left || position.x > right ||
		position.y < top || position.y > bottom;
}

bool Projectile::CollidesWith(const Vector2<float>& otherPos, float otherRadius) const
{
	return (position - otherPos).Length() < radius + otherRadius;
}