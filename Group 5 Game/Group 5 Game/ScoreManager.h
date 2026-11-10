#pragma once

#include <iostream>
#include "DifficultyManager.h"

class ScoreManager
{
public:
	void Update(float deltaTime)
	{
		time += deltaTime;
	}
	void AddScore(int amount)
	{
		score += amount * scoreMultiplier;
	}

	void Reset()
	{
		time = 0.0f;
		score = 0;
	}

	void SetScoreMultiplier(DifficultyType difficulty)
	{
		switch (difficulty)
		{
		case DifficultyType::Easy:
			scoreMultiplier = 0.85f;
			break;
		case DifficultyType::Medium:
			scoreMultiplier = 1.0f;
			break;
		case DifficultyType::Hard:
			scoreMultiplier = 1.15f;
			break;
		}
	}

	float GetTime()
	{
		return time;
	}
	int GetScore()
	{
		return score;
	}
private:
	float time = 0.0f;
	int score = 0;

	float scoreMultiplier = 1.0f;
};