#pragma once

enum class DifficultyType
{
	Easy,
	Medium,
	Hard
};

class DifficultyManager
{
public:
	DifficultyManager()
	{
		currentDifficulty = DifficultyType::Medium;
	}

	DifficultyType GetDifficultyType()
	{
		return currentDifficulty;
	}
	void SetDifficultyType(DifficultyType newDifficulty)
	{
		currentDifficulty = newDifficulty;
	}
private:
	DifficultyType currentDifficulty;
};