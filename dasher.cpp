#include "raylib.h"

int main()
{
    // Window dimensions
    const int window_width = 800;
    const int window_height = 450;
    InitWindow(window_width, window_height, "Dapper-Dasher");

    // Acceleration due to gravity (pixels/frame/frame)
    const int gravity = 1;

    // Rectangle dimensions
    const int rect_width = 50;
    const int rect_height = 80;

    // Rectangle movement
    int posY = window_height - rect_height;
    int velocity = 0;
    bool isInAir = false;
    const int jump_velocity = -20;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // ***** Game Logic Begins *****

        // Ground check
        if (posY >= window_height - rect_height)
        {
            isInAir = false;
            velocity = 0;
        }
        else
        {
            isInAir = true;
            // Apply gravity
            velocity += gravity;
        }
        
        // Jump
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jump_velocity;
        }
        // Update Y postion
        posY += velocity;

        // ***** Game Logic Ends *****
        DrawRectangle(400, posY, rect_width, rect_height, BLUE);
        // Stop Drawing
        EndDrawing();
    }
    CloseWindow();
}