#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "cLDtk/cLDtk.h"

#include "game.h"
#include "constants.h"

#define MAX_LAYERS 8

static const f32 G = 10.0f;
static const f32 JUMP_SPEED = 512.0f;

static struct {
    v2 pos;
    v2 vel;
    bool on_ground;
    bool is_jumping;
} player;

static Rectangle obj  = {
    .x = 0, .y = 200, .width = 100, .height = 20,
}, resolution = {0};

static struct {
    i8 offset;
    Texture2D tex;

    struct levels *_level;
    struct layerInstances *tiles;
    struct layerInstances *collisions;
} level;

void DrawTiles();
void DoCollision(Rectangle obstacle);

void GameInit() {
    player.pos = Vector2Zero();
    player.vel = Vector2Zero();
    player.on_ground = false;
    player.is_jumping = false;

    level.tex = LoadTexture("assets/tileset.png");
    level.offset = 8;
    loadJSONFile("{\"jsonVersion\":\"\"}","assets/level.json");

    importMapData();

    level._level = getLevel("Level_0");
    level.collisions = getLayer("Collisions", level._level->uid);

    printf("%d\n", level.collisions->gridSize);
}

void GameDeinit() {
    UnloadTexture(level.tex);
    freeMapData();
    json_value_free(schema);
    json_value_free(user_data);
}

void GameUpdate(f32 dt) {
    if (IsKeyDown(KEY_SPACE) && !player.is_jumping) {
        player.vel.y = -JUMP_SPEED;
        player.on_ground = false;
        player.is_jumping = true;
    }

    // Debugging purposes
    if (IsKeyDown(KEY_W))
        player.pos.y -= PLAYER_SPEED * dt;

    if (IsKeyDown(KEY_S))
        player.pos.y += PLAYER_SPEED * dt;

    int direction = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    if (direction != 0) {
        player.vel.x += PLAYER_SPEED * direction;
    } else
        player.vel.x = Lerp(player.vel.x, 0, 1.f);

    // gravity
    if (!player.on_ground)
        player.vel.y += G;
    else {
        player.vel.y -= 2.0f;
    }

    player.vel.x = Clamp(player.vel.x, -256, 256);

    // collision
    player.on_ground = false;

    struct layerInstances *layer = level.collisions;
    for (i32 y = 0; y < layer->autoTiles_data_ptr->count; y++) {
        struct autoTiles *tiles = layer->autoTiles_data_ptr + y;

        Rectangle obsta = { tiles->x, tiles->y,  layer->gridSize, layer->gridSize };
        DoCollision(obsta);
    }

    DoCollision(obj);

    if (player.pos.y > GAME_HEIGHT) {
        player.pos.y = 0;
    }

    player.vel.y = Clamp(player.vel.y, -1024, 512);

    player.pos = Vector2Add(player.pos, Vector2Scale(player.vel, dt));

}

void GameRender() {
    DrawRectangleV(player.pos, (Vector2){ 10, 10 }, WHITE);
    DrawRectangleRec(obj, WHITE);
    DrawRectangleLinesEx(resolution, 1.0f, RED);
    //
    const char *formated =
        TextFormat("x: %f, y: %f, w: %f, h: %f\nv: %f", resolution.x, resolution.y,
                   resolution.width, resolution.height, player.vel.y);

    DrawText(formated, 0, 0, 10, WHITE);

    DrawTiles();
}

void DrawSprite(Texture2D texture, Rectangle sourceRec, v2 position, int flip, Color tint)
{
    switch(flip){
        case 0 :
            break;

        case 1 :
            sourceRec.width *= -1;
            break;

        case 2 :
            sourceRec.height *= -1;
            break;

        case 3 :
            sourceRec.width *= -1;
            sourceRec.height *= -1;
            break;
    }

    DrawTextureRec(texture, sourceRec, position, tint);
}

void DrawTiles() {
    struct layerInstances *layer = level.collisions;
    for (i32 y = 0; y < layer->autoTiles_data_ptr->count; y++) {
        struct autoTiles *tiles = layer->autoTiles_data_ptr + y;

        DrawSprite(level.tex,
                   Rect(tiles->SRCx, tiles->SRCy, level.offset, level.offset),
                   Vec2(tiles->x, tiles->y), tiles->f, WHITE);
    }
}

void DoCollision(Rectangle obstacle) {
    if (CheckCollisionRecs(Rect(player.pos.x, player.pos.y, 10, 10), obstacle)) {
        resolution = GetCollisionRec(Rect(player.pos.x, player.pos.y, 10, 10), obstacle);

        // in Y-Axis
        if (resolution.height < resolution.width) {
            if (player.pos.y < resolution.y) { /* bottom part */
                player.pos.y -= resolution.height;
                player.on_ground = true;
                player.is_jumping = false;
            } else
                player.pos.y += resolution.height;

            player.vel.y = 0;
        } else if (resolution.width < resolution.height) { /* X-axis */
            if (player.pos.x < resolution.x)
                player.pos.x -= resolution.width;
            else
                player.pos.x += resolution.width;

            player.vel.x = 0;
        }
    }
}
