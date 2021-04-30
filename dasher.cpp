#include "raylib.h"

int main()
{
    //window dimensions
    const int windowWidth{512};
    const int windowHeight{380};
    // initialize the window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

    // acceleration due to gravity (pixels/frame)/frame
    const int gravity{1};

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // scarfy dimensions
    const int width{scarfy.width/6};
    const int height{scarfy.height};

    Rectangle scarfyRec;
    scarfyRec.width = width;
    scarfyRec.height = height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = 0;
    scarfyPos.y = 0;

    // is the rectanlge in the air?
    bool isInAir{};
    // jump velocity
    const int jumpVel{-22};

    int posY{windowHeight - height};
    int velocity{0};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // perform ground check
        if (posY >= windowHeight - height)
        {
            // rectangle is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        // update the position
        posY += velocity;

        DrawRectangle(windowWidth/2, posY, width, height, BLUE);

        // stop drawing
        EndDrawing();
    }
    CloseWindow();
}