#include "raylib.h"

int main()
{
    //window dimensions
    const int windowWidth{512};
    const int windowHeight{380};
    // initialize the window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // stop drawing
        EndDrawing();
    }
    CloseWindow();
}