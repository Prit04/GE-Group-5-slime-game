
#pragma once
#include "GamesEngineeringBase.h"
#include "Vector.h"

class ScoreManager;

// Random crystal spawning system
class Collectible
{
private:
    static const int MAX_CRYSTALS = 36;
    float radius = 10.0f;

    struct Crystal
    {
        int x;
        int y;
        bool active;
    };

    Crystal crystals[MAX_CRYSTALS] = {};

    unsigned int seed = 0x4D595DF4u;
    float spawnTimer = 0.0f;
    bool spawning = false;


    // Generate a random number
    unsigned int random();

    // Spawn one crystal at a random position
    void spawnCrystal(int width, int height);

    bool CollidesWith(const Vector2<float>& otherPos, const Vector2<float>& position, float otherRadius);

public:

    // Start automatic spawning
    void Start();

    // Update the automatic spawn timer
    void Update(float dt, int width, int height);

    // Draw crystals
    void Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera);

    void CheckHit(const Vector2<float>& pos, float radius, ScoreManager& scoreManager);

    void Clear();
};
