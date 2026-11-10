#include <iostream>

#include "Screen.h"
#include "GameManager.h"

#include "GameManager.h"
#include <cstring>



// Draw pixel-style text
static void DrawText(
    GamesEngineeringBase::Window& window,
    const char* text, int x, int y, int scale,
    unsigned char r, unsigned char g, unsigned char b)
{
    static const char* letters = "GAMEOVRSTNUCIP";

    static const unsigned char patterns[14][7] = {
        { 14,17,16,23,17,17,15 }, // G
        { 14,17,17,31,17,17,17 }, // A
        { 17,27,21,21,17,17,17 }, // M
        { 31,16,16,30,16,16,31 }, // E
        { 14,17,17,17,17,17,14 }, // O
        { 17,17,17,17,17,10,4 },  // V
        { 30,17,17,30,20,18,17 }, // R
        { 15,16,16,14,1,1,30 },   // S
        { 31,4,4,4,4,4,4 },       // T
        { 17,25,21,19,17,17,17 }, // N
        { 17,17,17,17,17,17,14 }, // U
        { 15,16,16,16,16,16,15 }, // C
        { 31,4,4,4,4,4,31 },      // I
        { 30,17,17,30,16,16,16 }  // P
    };

    for (const char* p = text; *p; ++p)
    {
        const char* found = std::strchr(letters, *p);

        if (found)
        {
            int index = static_cast<int>(found - letters);

            for (int row = 0; row < 7; ++row)
            {
                for (int col = 0; col < 5; ++col)
                {
                    if (!(patterns[index][row] & (16 >> col)))
                        continue;

                    for (int dy = 0; dy < scale; ++dy)
                    {
                        for (int dx = 0; dx < scale; ++dx)
                        {
                            int px = x + col * scale + dx;
                            int py = y + row * scale + dy;

                            if (px >= 0 && py >= 0 &&
                                px < static_cast<int>(window.getWidth()) &&
                                py < static_cast<int>(window.getHeight()))
                            {
                                window.draw(px, py, r, g, b);
                            }
                        }
                    }
                }
            }
        }

        x += 6 * scale;
    }
}


// Draw a rectangular button
static void DrawButton(
    GamesEngineeringBase::Window& window,
    int x, int y, int width, int height,
    unsigned char r, unsigned char g, unsigned char b)
{
    for (int py = y; py < y + height; ++py)
    {
        for (int px = x; px < x + width; ++px)
        {
            if (px >= 0 && py >= 0 &&
                px < static_cast<int>(window.getWidth()) &&
                py < static_cast<int>(window.getHeight()))
            {
                window.draw(px, py, r, g, b);
            }
        }
    }
}

// Standard init for loading images etc
void GameOverScreen::Init()
{
	ScoreManager scoreManager = gameManager.GetScoreManager();
	finalScore = scoreManager.GetScore();
	timeAlive = scoreManager.GetTime();

	// call once setting all stuff needed to display in gameover
	scoreManager.Reset();
	std::cout << "Game Over Screen Loaded" << std::endl;

// Reset the mouse click state
    wasMouseDown = window.mouseButtonPressed(
        GamesEngineeringBase::MouseLeft);
}

// Any inputs needed, add here
void GameOverScreen::HandleInput()
{


    // Get the mouse position
    int mx = window.getMouseInWindowX();
    int my = window.getMouseInWindowY();

    // Check the left mouse button
    bool mouseDown = window.mouseButtonPressed(
        GamesEngineeringBase::MouseLeft);

    // Trigger only when the mouse is newly pressed
    if (mouseDown && !wasMouseDown)
    {

        if (mx >= 362 && mx < 662 &&
            my >= 510 && my < 566)
        {
            gameManager.SetState(GameState::Game);
        }
 
        else if (mx >= 362 && mx < 662 &&
            my >= 590 && my < 646)
        {
            gameManager.SetState(GameState::Menu);
        }
    }

    wasMouseDown = mouseDown;


}

// Update logic here
void GameOverScreen::Update(float deltaTime)
{

}

// Draw calls here
void GameOverScreen::Draw(GamesEngineeringBase::Window& window)
{

    int centerX = static_cast<int>(window.getWidth()) / 2;

    // Draw a dark background
    for (int y = 0; y < static_cast<int>(window.getHeight()); ++y)
    {
        for (int x = 0; x < static_cast<int>(window.getWidth()); ++x)
        {
            window.draw(x, y, 0, 0, 0);
        }
    }


    DrawText(window, "GAME OVER",centerX - 162, 170, 6, 255, 105, 70);

    DrawText(window, "SCORE", 250, 320, 4, 180, 220, 255);

    resultRenderer.drawWholeNumber(window, finalScore, 500, 320);

    DrawText(window, "TIME",250, 420, 4, 180, 220, 255);

    resultRenderer.drawTime(
        window, static_cast<int>(timeAlive), 500, 420);

    DrawButton(window, 362, 510, 300, 56, 255, 105, 70);

    DrawText(window, "RESTART",449, 528, 3, 255, 255, 255);

    DrawButton(window, 362, 590, 300, 56, 52, 68, 95);

    DrawText(window, "MAIN MENU",431, 608, 3, 255, 255, 255);

}

// Destroy pointers etc, any stuff that would cause a mem leak
void GameOverScreen::Destroy()
{

}