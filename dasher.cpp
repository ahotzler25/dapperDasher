#include "raylib.h"

struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    int vel;
};

bool isOnGround(AnimData data, int windowHeight) {
    return data.pos.y >= windowHeight - data.rec.height;
};

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame) {
    // update runningTime
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime) {
        data.runningTime = 0.0;

        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame) {
            data.frame = 0;
        }
    }

    return data;
};

int main() {

    // Window Dimensions as an array
    int windowDimensions[2];
        windowDimensions[0] = 512; // windowWidth
        windowDimensions[1] = 380; // windowHeight
        
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

    // acceleration due to gravity (pixels/second)/second
    const int gravity{1'000};

    // Jump velocity (pixels/second)
    const int jumpVel{-600};

    // Scarfy Stuff
    Texture2D scarfy = LoadTexture("textures/scarfy.png");

    // AnimData Struct
    AnimData scarfyData;
        scarfyData.rec.x = 0;
        scarfyData.rec.y = 0;
        scarfyData.rec.width = scarfy.width / 6;
        scarfyData.rec.height = scarfy.height;
        scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
        scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
        scarfyData.frame = 0;
        scarfyData.updateTime = 1.0/12.0;
        scarfyData.runningTime = 0.0;
        scarfyData.vel = 0;

    // Nebula Stuff
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{5};

    // This initializes the array elements on same line of declaration
    AnimData nebulae[sizeOfNebulae]{};
    
    // For loop to initialize AnimData member variables
    for (int i = 0; i < sizeOfNebulae; i++) {
        nebulae[i].rec.x = 0;
        nebulae[i].rec.y = 0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/12.0;
        nebulae[i].vel = -200;
        nebulae[i].pos.x = windowDimensions[0] + (i * 300);
    };

    // X position of finishLine
    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };


    // This is an alt way to intialize a struct
    // AnimData nebData{
    //     { 0, 0, nebula.width / 8, nebula.height / 8 }, // Rectangle rec
    //     { windowWidth + 600, windowHeight - nebula.height / 8 - 10 }, // Vector2 pos; is nebula.height / 8 the best way to do this?
    //     0, // int frame
    //     1.0/12.0, // float updateTime
    //     0.0, // float runningTime
    //     -180 // int vel
    // };

    bool isInAir = true;

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    // Checks for scarfy and neb collisions
    bool collision{};

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        
        // Delta Time (time since last frame)
        const float dT{ GetFrameTime()};

        // Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // background x coordinate/pos
        // Scroll the background
        bgX -= 20 * dT;
        if (bgX <= -background.width * 2) {
            bgX = 0.0;
        };

        // Scroll the midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2) {
            mgX = 0.0;
        };

        // Scroll the foreground
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2) {
            fgX = 0.0;
        }

        // Draw background
        Vector2 bg1Pos{ bgX, 0.0 };
        Vector2 bg2Pos{ bgX + background.width * 2, 0.0 };
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // Draw midground
        Vector2 mg1Pos { mgX, 0.0 };
        Vector2 mg2Pos { mgX + midground.width * 2, 0.0 };
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // Draw foreground
        Vector2 fg1Pos { fgX, 0.0 };
        Vector2 fg2Pos { fgX + foreground.width * 2, 0.0 };
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);


        // perform ground check
        if (isOnGround(scarfyData, windowDimensions[1])) {
            // rectangle is on the ground
            scarfyData.vel = 0;
            isInAir = false;
        } else {
            // rectangle is in the air
            scarfyData.vel += gravity * dT;
            isInAir = true;
        }


        // JUMP LOGIC
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {
            scarfyData.vel += jumpVel;
        }

        // Update Scarfy Position every frame
        scarfyData.pos.y += scarfyData.vel * dT;

        // Update Scarfy Animation Frame
        if (!isInAir) {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        };

        // For loop to update nebulae[i].pos.x positions
        for (int i = 0; i < sizeOfNebulae; i++) {
            nebulae[i].pos.x += nebulae[i].vel * dT;
        };

        // Range based for loop; similar to JavaScript's forEach
        for (AnimData nebula : nebulae) {
            float pad{50};
            Rectangle nebRec {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad
            };

            Rectangle scarfyRec {
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec)) {
                collision = true;
            }
            
        };



        // For loop to update nebulae animation frames
        for (int i = 0; i < sizeOfNebulae; i++) {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        };

        // Update finishLine x pos
        finishLine += nebulae[sizeOfNebulae - 1].vel * dT;

        if (collision) {
            // Lose the game
            bgX = 0;
            mgX = 0;
            fgX = 0;
            DrawText("Game Over!", windowDimensions[0] / 4, 20, 60, RED);
        } else if (scarfyData.pos.x >= finishLine + 200) {
            // Win the game
            bgX = 0;
            mgX = 0;
            fgX = 0;
            DrawText("You Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 60, BLUE);
        } else {
            // For loop to draw nebulae textures
            for (int i = 0; i < sizeOfNebulae; i++) {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            
            // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        // Stop Drawing
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    // Apparently this is the correct way to close a window
    CloseWindow();
}