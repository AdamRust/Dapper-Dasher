#include "raylib.h"

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float update_time;
    float running_time;
};

bool isOnGround(AnimData data, int window_height)
{
    return data.pos.y >= window_height - data.rect.height;
}

AnimData updateAnimData(AnimData data, int frame_max, float delta_time)
{
    // Update running time
    data.running_time += delta_time;
    // Update animation frame
    if (data.running_time >= data.update_time)
    {
        // Reset running time
        data.running_time = 0.0;
        // Move scarfy rect to next frame
        data.rect.x = data.frame * data.rect.width;
        // Update frame
        data.frame++;
        if (data.frame > frame_max)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // Window dimensions
    int window_dimensions[2];
    window_dimensions[0] = 512;
    window_dimensions[1] = 380;
    InitWindow(window_dimensions[0], window_dimensions[1], "Dapper-Dasher");

    // Acceleration due to gravity (pixels/s)/s
    const int GRAVITY = 1000;

    // ***** Nebula *****
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    // Nebula constants
    const int NEBULAE_SHEET_FRAME_WIDTH = 8;
    const int NUM_OF_NEBULAE = 10;
    const int NEBULAE_SPACING = 300;
    // Nebulae AnimData
    AnimData nebulae[NUM_OF_NEBULAE]{};
    // Initialize nebulae
    for (int i = 0; i < NUM_OF_NEBULAE; i++)
    {
        nebulae[i] =
        {
            {0.0, 0.0, static_cast<float>(nebula.width / NEBULAE_SHEET_FRAME_WIDTH), static_cast<float>(nebula.height / NEBULAE_SHEET_FRAME_WIDTH)},
            {static_cast<float>(window_dimensions[0] + (i * NEBULAE_SPACING)), static_cast<float>(window_dimensions[1] - nebula.height / NEBULAE_SHEET_FRAME_WIDTH)},
            0,
            1.0 / 12.0,
            0.0
        };
    };
    // Nebula velocity (pixels/second)
    const int NEBULA_VEL = -200;

    // ***** Scarfy *****
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // Scarfy constants
    const int SCARFY_SHEET_FRAME_WIDTH = 6;
    // Scarfy AnimData
    AnimData scarfy_data
    {
        {0.0, 0.0, static_cast<float>(scarfy.width / SCARFY_SHEET_FRAME_WIDTH), static_cast<float>(scarfy.height)},
        {static_cast<float>((window_dimensions[0] / 2) - (scarfy_data.rect.width / 2)), static_cast<float>(window_dimensions[1] - scarfy.height)},
        0,
        1.0 / 12.0,
        0.0
    };
    // Scarfy movement
    int velocity = 0;
    bool isInAir = false;
    // Jump velocity (pixels/s)
    const int JUMP_VEL = -600;

    // ***** Background *****
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float background_x = 0.0;
    // Backgorund constants
    const int BACKGROUND_VEL = -20;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // Get time since last frame
        const float dT = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        background_x += BACKGROUND_VEL * dT;
        if (background_x <= -background.width * 2)
        {
            background_x = 0.0;
        }

        // Draw background
        Vector2 background_pos_1{background_x, 0.0};
        DrawTextureEx(background, background_pos_1, 0.0, 2.0, WHITE);
        Vector2 background_pos_2{background_x + (background.width * 2), 0.0};
        DrawTextureEx(background, background_pos_2, 0.0, 2.0, WHITE);

        // Ground check
        if (isOnGround(scarfy_data, window_dimensions[1]))
        {
            isInAir = false;
            velocity = 0;
        }
        else
        {
            isInAir = true;
            // Apply gravity
            velocity += GRAVITY * dT;
        }
        
        // Check for jump
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += JUMP_VEL;
        }

        // Update Scarfy postion
        scarfy_data.pos.y += velocity * dT;

        // Animate Scarfy
        if (!isInAir)
        {
            scarfy_data = updateAnimData(scarfy_data, SCARFY_SHEET_FRAME_WIDTH - 1, dT);
        }
        // Draw Scarfy
        DrawTextureRec(scarfy, scarfy_data.rect, scarfy_data.pos, WHITE);

        // Animate and draw nebulae
        for (int i = 0; i < NUM_OF_NEBULAE; i++)
        {
            // Animate nebula
            nebulae[i] = updateAnimData(nebulae[i], NEBULAE_SHEET_FRAME_WIDTH - 1, dT);
            nebulae[i].pos.x += NEBULA_VEL * dT;
            // Draw nebula
            DrawTextureRec(nebula, nebulae[i].rect, nebulae[i].pos, WHITE);
        }

        EndDrawing();
    }
    // Unload Nebula texture
    UnloadTexture(nebula);
    // Unload Scarfy texture
    UnloadTexture(scarfy);
    // Unload background texture
    UnloadTexture(background);
    CloseWindow();
}