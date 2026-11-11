#include <algorithm>

#include "Slime.h"

// One entry per animation, in the same order as the enum
const Slime::Animation Slime::Animations[Slime::AnimationCount] =
{
	{ 0, 4,  6.0f, true  },		// idle
	{ 1, 6, 10.0f, true  },		// walk
	{ 2, 6, 10.0f, false },		// hurt
	{ 3, 6,  8.0f, false }		// death
};

bool Slime::Load(const std::string& filename)
{
	return sheet.load(filename);
}

void Slime::Spawn(const Vector2<float>& spawnPosition)
{
	position = spawnPosition;
	startPosition = spawnPosition;
	hurtTimer = 0.0f;
	SetState(State::Idle);
	scale = 1.0f;
	targetScale = 1.0f;
}

void Slime::SetPosition(const Vector2<float>& newPosition)
{
	position = newPosition;
}

Vector2<float> Slime::GetPosition() const
{
	return position;
}

bool Slime::IsDead() const
{
	return state == State::Death;
}

// Called when the slime takes a hit. Ignored while it is already dead.
void Slime::Hurt()
{
	if (state == State::Death)
	{
		return;
	}

	hurtTimer = HurtDuration;
	SetState(State::Hurt);
}

// Plays the death animation once and stays on the last frame
void Slime::Die()
{
	if (state == State::Death)
	{
		return;
	}

	hurtTimer = 0.0f;
	SetState(State::Death);
}

// Puts the slime back at its starting point, ready to play again
void Slime::Reset()
{
	position = startPosition;
	hurtTimer = 0.0f;
	SetState(State::Idle);
}

void Slime::Update(float deltaTime, const Vector2<float>& moveInput)
{
	if (dashTimer > 0)
	{
		dashTimer -= deltaTime;
		if (dashTimer <= 0)
		{
			canDash = true;
		}
	}

	if (dashDuration > 0)
	{
		dashDuration -= deltaTime;
		if (dashDuration <= 0)
		{
			isDashing = false;
		}
	}

	if (isDashing)
	{
		speedMultiplier = 2.5f;
	}
	else
	{
		speedMultiplier = 1.0f;
	}

	// Move the size a bit closer to the target each frame, for a smooth grow/shrink
	scale += (targetScale - scale) * std::min(1.0f, ScaleSpeed * deltaTime);

	AdvanceFrame(deltaTime);

	// A dead slime does not move or animate any further
	if (state == State::Death)
	{
		return;
	}

	// While recovering from a hit the slime cannot move
	if (hurtTimer > 0.0f)
	{
		hurtTimer -= deltaTime;

		if (hurtTimer <= 0.0f)
		{
			hurtTimer = 0.0f;
			SetState(State::Idle);
		}

		return;
	}

	Move(deltaTime, moveInput);
}

void Slime::Move(float deltaTime, const Vector2<float>& moveInput)
{
	// No input means no movement, so go back to the idle animation
	if (moveInput.Length() == 0.0f)
	{
		SetState(State::Idle);
		return;
	}

	const Vector2<float> direction = moveInput.Normalised();
	position = position + direction * (moveSpeed * speedMultiplier * deltaTime);

	SetState(State::Walk);
}

bool Slime::TryDash()
{
	if (!canDash)
	{
		return false;
	}
	if (targetScale <= 0.6f)
	{
		return false;
	}

	targetScale -= 0.2f;
	canDash = false;
	isDashing = true;
	dashTimer = dashCooldown;
	dashDuration = 1.0f;

	return true;
}

void Slime::Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera) const
{
	const Animation& animation = Animations[CurrentAnimation()];

	// The frame we want inside the sheet
	const int sourceX = frame * FrameSize;
	const int sourceY = animation.row * FrameSize;

	// Screen position: world position minus the camera, anchored on the feet
	const int screenX = static_cast<int>(position.x - FeetX * scale - camera.x);
	const int screenY = static_cast<int>(position.y - FeetY * scale - camera.y);

	DrawImageRegionScaled(window, sheet, sourceX, sourceY, FrameSize, FrameSize, screenX, screenY, scale);
}

void Slime::SetState(State newState)
{
	if (state == newState)
	{
		return;
	}

	state = newState;
	frame = 0;
	frameTime = 0.0f;
}

void Slime::AdvanceFrame(float deltaTime)
{
	const Animation& animation = Animations[CurrentAnimation()];

	frameTime += deltaTime;

	// Frames are picked from time, not from rendered frames, so the animation
	// plays at the same speed whatever the frame rate is
	const int frameIndex = static_cast<int>(frameTime * animation.fps);

	if (animation.loop)
	{
		frame = frameIndex % animation.frames;
	}
	else
	{
		frame = std::min(frameIndex, animation.frames - 1);
	}
}

int Slime::CurrentAnimation() const
{
	return static_cast<int>(state);
}

void Slime::SetTargetScale(float newScale)
{
	targetScale = newScale;
}

float Slime::GetScale() const
{
	return scale;
}
