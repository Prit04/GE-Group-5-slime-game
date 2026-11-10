#pragma once
#include "GamesEngineeringBase.h"
#include "Vector.h"

class Projectile
{
public:
	void Activate(const Vector2<float>& _position, const Vector2<float>& _velocity, float _radius);
	void Deactivate();
	bool IsActive() const;

	void Update(float deltaTime);

	// camera is subtracted so the projectile is drawn in world space
	void Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera) const;

	// true if the projectile is outside the given world-space rectangle
	bool IsOutside(float left, float top, float right, float bottom) const;

	// circle vs circle check, distance between centres < radii added together
	bool CollidesWith(const Vector2<float>& otherPos, float otherRadius) const;

private:
	Vector2<float> position;
	Vector2<float> velocity;
	float radius = 8.0f;
	bool active = false;
};