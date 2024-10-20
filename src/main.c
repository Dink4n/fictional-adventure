#include <raylib.h>
#include <raymath.h>

#include "constants.h"
#include "types.h"

#include "game.h"

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");

    RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);

    GameInit();
    while (!WindowShouldClose()) {
        float scale = MIN((float)GAME_WIDTH/GetScreenWidth(), (float)GAME_HEIGHT/GetScreenHeight());


        Vector2 mouse = GetMousePosition();
        SetMouseScale(scale, scale);

        GameUpdate(GetFrameTime());

        // Render
        BeginTextureMode(target);
            ClearBackground(BLACK);
            GameRender();
        EndTextureMode();

        BeginDrawing();

        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
        DrawTexturePro(target.texture,
                       (Rectangle){0, 0, (float)target.texture.width,
                                   (float)-target.texture.height},
                       (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()},
                       (Vector2){0, 0}, 0.0f, WHITE);

        EndDrawing();
    }

    return 0;
}
