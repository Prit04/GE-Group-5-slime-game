#pragma once

#include <string>

#include "GamesEngineeringBase.h"
#include "Renderer.h"
#include "Vector.h"

// -------------------------------------------------------------------------
// The slime character.
//
// The sprite sheet has one animation per row, so the class is a small state
// machine plus a table that says which row to play and how fast:
//
//   row 0  idle   (4 frames)
//   row 1  walk   (6 frames)
//   row 2  hurt   (6 frames)
//   row 3  death  (6 frames)
//
// Position is the point under the middle of the slime (its feet), so drawing
// simply offsets the frame by a fixed amount.
// -------------------------------------------------------------------------

class Slime
{
public:

	enum class State
	{
		Idle,
		Walk,
		Hurt,
		Death
	};

	// Loads the sprite sheet. Returns false if the file is missing.
	bool Load(const std::string& filename);

	// moveInput is the direction the player wants to go this frame
	void Update(float deltaTime, const Vector2<float>& moveInput);

	// camera is subtracted so the slime is drawn in world space
	void Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera) const;

	void Hurt();
	void Die();
	void Reset();

	// Spawn places the slime and remembers where "home" is, so Reset can go back
	void Spawn(const Vector2<float>& spawnPosition);
	void SetPosition(const Vector2<float>& newPosition);
	Vector2<float> GetPosition() const;
	bool IsDead() const;

	// Size of the slime (1.0 = normal). The slime grows/shrinks smoothly toward the target
	void SetTargetScale(float newScale);
	float GetScale() const;

private:

	// One row of the sheet
	struct Animation
	{
		int row;		// which row of the sheet
		int frames;		// how many frames it has
		float fps;		// playback speed
		bool loop;		// true for idle / walk, false for hurt / death
	};

	static const int FrameSize = 50;		// every frame is 50x50
	static const int AnimationCount = 4;

	// Feet point inside a frame (the slime art sits around x 25, bottom y 45)
	static const int FeetX = 25;
	static const int FeetY = 45;

	static const Animation Animations[AnimationCount];

	void SetState(State newState);
	void AdvanceFrame(float deltaTime);
	void Move(float deltaTime, const Vector2<float>& moveInput);
	int CurrentAnimation() const;

	GamesEngineeringBase::Image sheet;

	Vector2<float> position{ 400.0f, 300.0f };
	Vector2<float> startPosition{ 400.0f, 300.0f };

	State state = State::Idle;
	int frame = 0;
	float frameTime = 0.0f;
	float hurtTimer = 0.0f;

	float moveSpeed = 120.0f;		// pixels per second

	float scale = 1.0f;          // current drawn size
	float targetScale = 1.0f;    // size it's growing/shrinking toward
	static constexpr float ScaleSpeed = 8.0f;   // how quickly it changes size

public:

	// How long the hurt pose is held before going back to normal
	static constexpr float HurtDuration = 0.5f;
};
