#pragma once

#include <cmath>

template <typename T>
class Vector2
{
public:
	T x;
	T y;

	Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2(T _x, T _y)
	{
		x = _x;
		y = _y;
	}

	// Set a new vector2
	void Set(T _x, T _y)
	{
		x = _x;
		y = _y;
	}

	float Length() const
	{
		return std::sqrt(x * x + y * y);
	}

	T Dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2 Normalised() const
	{
		float length = Length();

		if (length == 0)
		{
			return Vector2(0, 0);
		}

		return Vector2(x / length, y / length);
	}

	// Operators for addition, subtraction, and multiplication
	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}
};