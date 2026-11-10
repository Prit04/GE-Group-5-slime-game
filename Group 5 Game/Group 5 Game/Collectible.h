
#pragma once
#include "GamesEngineeringBase.h"
#include "Vector.h"


// Random crystal spawning system
class Collectible
{
private:
    static const int MAX_CRYSTALS = 36;

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
};
