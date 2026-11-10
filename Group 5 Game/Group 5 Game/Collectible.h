
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


    // Spawn a crystal near the player
    void spawnCrystal(const Vector2<float>& playerPos,
        int worldWidth, int worldHeight);

    bool CollidesWith(const Vector2<float>& otherPos, const Vector2<float>& position, float otherRadius);

public:

    // Start automatic spawning
    void Start();


    // Update crystal spawning using the player position
    void Update(float dt, const Vector2<float>& playerPos,
        int worldWidth, int worldHeight);


    // Check whether the player collects a crystal
    void CheckPickup(const Vector2<float>& playerPos);

    // Draw crystals
    void Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera);

    void CheckHit(const Vector2<float>& pos, float radius, ScoreManager& scoreManager);

    void Clear();
};
