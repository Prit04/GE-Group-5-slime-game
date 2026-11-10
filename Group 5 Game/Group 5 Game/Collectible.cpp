
#include "Collectible.h"
#include <cstdlib>

//  the xorshift random algorithm 
unsigned int Collectible::random()
{
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}


// Start automatic crystal spawning after pressing G
void Collectible::Start()
{
    spawning = true;
}


// Spawn one crystal at a random position
void Collectible::spawnCrystal(int width, int height)
{
    if (width < 32 || height < 32)
        return;

    for (int i = 0; i < MAX_CRYSTALS; i++)
    {
        if (!crystals[i].active)
        {
            crystals[i].x = random() % (width - 24);
            crystals[i].y = random() % (height - 24);
            crystals[i].active = true;
            return;
        }
    }
}


// Spawn one crystal every 3 seconds after activation
void Collectible::Update(float dt, int width, int height)
{
    if (!spawning)
        return;

    spawnTimer += dt;

    if (spawnTimer >= 3.0f)
    {
        spawnTimer = 0.0f;
        spawnCrystal(width, height);
    }
}


// Reuse the blue pixel crystal appearance from my game
void Collectible::Draw(GamesEngineeringBase::Window& window)
{
    for (int i = 0; i < MAX_CRYSTALS; i++)
    {
        if (!crystals[i].active)
            continue;

        int ox = crystals[i].x;
        int oy = crystals[i].y;

        for (int y = 1; y <= 21; y++)
        {
            for (int x = 1; x <= 21; x++)
            {
                int d = abs(x - 11) + abs(y - 11);

                if (d > 10)
                    continue;


                // Draw crystal facets using different shades of blue
                if (d >= 9)
                    window.draw(ox + x, oy + y, 145, 103, 79);
                else if (x < 11 && y < 11)
                    window.draw(ox + x, oy + y, 151, 225, 247);
                else if (x >= 11 && y < 11)
                    window.draw(ox + x, oy + y, 220, 247, 251);
                else if (x < 11)
                    window.draw(ox + x, oy + y, 89, 183, 224);
                else
                    window.draw(ox + x, oy + y, 122, 209, 233);
            }
        }
    }
}
