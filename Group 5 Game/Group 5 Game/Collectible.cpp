
#include "Collectible.h"
#include "ScoreManager.h"
#include <cstdlib>
#include <algorithm>

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



// Spawn crystals at random positions near the player
void Collectible::spawnCrystal(
    const Vector2<float>& playerPos,
    int worldWidth, int worldHeight)
{
    for (int i = 0; i < MAX_CRYSTALS; i++)
    {
        if (!crystals[i].active)
        {
       
            // Choose a random position around the player
            for (int attempt = 0; attempt < 30; attempt++)
            {
                int dx = static_cast<int>(random() % 401) - 200;
                int dy = static_cast<int>(random() % 401) - 200;

             
                // Avoid spawning too close to the player
                if (dx * dx + dy * dy < 70 * 70)
                    continue;

                int x = static_cast<int>(playerPos.x) + dx;
                int y = static_cast<int>(playerPos.y) + dy;

                // Keep crystals inside the world boundaries
                if (x < 0 || y < 0 ||
                    x + 22 >= worldWidth ||
                    y + 22 >= worldHeight)
                    continue;

                crystals[i].x = x;
                crystals[i].y = y;
                crystals[i].active = true;
                return;
            }

            return;
        }
    }
}


// Spawn a crystal near the player every 3 seconds
void Collectible::Update(
    float dt, const Vector2<float>& playerPos,
    int worldWidth, int worldHeight)
{
    if (!spawning)
        return;

    spawnTimer += dt;

    if (spawnTimer >= 3.0f)
    {
        spawnTimer = 0.0f;
        spawnCrystal(playerPos, worldWidth, worldHeight);
    }
}


// Reuse the blue pixel crystal appearance from my game
void Collectible::Draw(GamesEngineeringBase::Window& window, const Vector2<float>& camera)
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

                int screenX = ox + x - camera.x;
                int screenY = oy + y - camera.y;

                if (screenX < 0 || screenX >= window.getWidth() || screenY < 0 || screenY >= window.getHeight())
                {
                    continue;
                }

                // Draw crystal facets using different shades of blue
                if (d >= 9)
                    window.draw(screenX, screenY, 145, 103, 79);
                else if (x < 11 && y < 11)
                    window.draw(screenX, screenY, 151, 225, 247);
                else if (x >= 11 && y < 11)
                    window.draw(screenX, screenY, 220, 247, 251);
                else if (x < 11)
                    window.draw(screenX, screenY, 89, 183, 224);
                else
                    window.draw(screenX, screenY, 122, 209, 233);
            }
        }
    }
}

int Collectible::CheckHit(const Vector2<float>& otherPos, float otherRadius, ScoreManager& scoreManager)
{
    int collected = 0;

    for (int i = 0; i < MAX_CRYSTALS; i++)
    {
        if (!crystals[i].active)
        {
            continue;
        }

        // Crystals are drawn in a 22x22 box, so the centre is (x + 11, y + 11)
        Vector2<float> position = { static_cast<float>(crystals[i].x + 11), static_cast<float>(crystals[i].y + 11) };
        if (CollidesWith(otherPos, position, otherRadius))
        {
            scoreManager.AddScore(100);
            crystals[i].active = false;
            collected++;
        }
    }

    return collected;
}


bool Collectible::CollidesWith(const Vector2<float>& otherPos, const Vector2<float>& position, float otherRadius)
{
    return (position - otherPos).Length() < radius + otherRadius;
}

void Collectible::Clear()
{
    for (int i = 0; i < MAX_CRYSTALS; i++)
    {
        crystals[i].active = false;
    }
}