
#pragma once
#include "GamesEngineeringBase.h"


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

    // Spawn one crystal at a random position
    void spawnCrystal(int width, int height);

public:

    // Start automatic spawning
    void Start();

    // Update the automatic spawn timer
    void Update(float dt, int width, int height);

    // Draw crystals
    void Draw(GamesEngineeringBase::Window& window);
};
