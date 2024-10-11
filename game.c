#include "game.h"
#include "difficulty.h"
#include <stdlib.h>

#define G 1400
#define PLAYER_JUMP_SPD 500.0f
#define OBSTACLE_DISTANCE_INTERVAL 280
#define OBSTACLE_WIDTH 100
#define MIN_SIZE_OBS 300
#define INC_SCORE 0.5

Vector2 getSpriteCoordinates(Player player, float playerSpriteScale);

Rectangle getPlayerHitbox(Player player, float playerSpriteScale);

int play(Difficulty difficulty, int coop)
{
    // Player initialization
    Player player = { 0 };
    player.position = (Vector2){ 200, 200 };
    player.speed = 0;
    player.alive = 1;

    Player player2 = { 0 };
    if (coop)
    {
        player2.position = (Vector2) {200, 150};
        player2.speed = 0;
        player2.alive = 1;
    }

    // Load sprites
    Texture2D playerSprite = LoadTexture("resources/flappy.png");
    Texture2D player2Sprite = LoadTexture("resources/flappy_player2.png");
    Texture2D obstacleSprite = LoadTexture("resources/obstacle.png");
    Texture2D berrySprite = LoadTexture("resources/berry.png");

    float playerSpriteScale = 3.5F;

    player.spriteCoordinates = getSpriteCoordinates(player, playerSpriteScale);
    player.hitbox = getPlayerHitbox(player, playerSpriteScale);
    if (coop)
    {
        player2.spriteCoordinates = getSpriteCoordinates(player2, playerSpriteScale);
        player2.hitbox = getPlayerHitbox(player2, playerSpriteScale);
    }

    int currentThreshold = 1;
    int currentGap = difficulty.gap;
    int currentMaxHeightDiff = difficulty.maxHeigtDiff;
    int currentObstacleVelocity = difficulty.obstacleVelocity;
    srand(time(NULL));

    int upperObstacleHeight = MIN_SIZE_OBS + (rand() % ((700 - currentGap) - MIN_SIZE_OBS + 1));
    int lowerObstacleHeight = 800 - upperObstacleHeight -  currentGap;

    Obstacle obstacles[5] = {
        {{1200, 0, OBSTACLE_WIDTH, upperObstacleHeight}, {1200, (800-lowerObstacleHeight), OBSTACLE_WIDTH, lowerObstacleHeight}}
    };
    int numObstacles = 1;

    // Berry for revival
    Rectangle berry = {1200, 0, 100, 100};
    int berryInGame = 0;

    float score = 0;

    // Borders
    Rectangle floor = { 0, 800, 1200, 0 };
    Rectangle ceiling = { 0, 0, 1200, 0 };

    // Camera initialization
    Camera2D camera = { 0 };
    camera.target = (Vector2){ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 }; // Camera focus in the center of the screen
    camera.offset = (Vector2){ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };
    camera.zoom = 1.0f;

    int framesCounter = 0;
    int framesSpeed = 4;
    int player1Died = 0;
    int player2Died = 0;

    float invincibilityTimer = 0;

    SetTargetFPS(60);

    bool start = false;

    // Main game loop
    while (!WindowShouldClose())
    {
        if (start)
        {
            UpdatePlayer(&player, &player2, coop);
            UpdateObstacles(&obstacles, &numObstacles, currentMaxHeightDiff, upperObstacleHeight, currentGap, currentObstacleVelocity);
            UpdateScore(&score, difficulty, &currentThreshold, &currentGap, &currentMaxHeightDiff, &currentObstacleVelocity, player, player2, obstacles, &berry, &berryInGame, coop);
            UpdateBerry(&berry, &berryInGame, currentObstacleVelocity);
        } else if (IsKeyPressed(KEY_SPACE)) start = true;

        Draw(camera, obstacles, numObstacles, obstacleSprite, berrySprite, &player, &player2, playerSprite, player2Sprite, playerSpriteScale, score, start, coop, berry);

        // Timer
        if (player.invincible || player2.invincible)
        {
            invincibilityTimer += GetFrameTime();
        }
        if ((player.invincible || player2.invincible) && invincibilityTimer >= 3.0F)
        {
            player.invincible = 0;
            player2.invincible = 0;
            invincibilityTimer = 0;
            if (player.position.y < 0 || player.position.y > 800)
                player.alive = 0;
            if (player2.position.y < 0 || player.position.y > 800)
                player.alive = 0;
        }

        // Player death animations
        if (player1Died)
        {
            framesCounter++;
            if (framesCounter >= (60/framesSpeed))
            {
                framesCounter = 0;
                BeginDrawing();
                    DrawTextureEx(playerSprite, player.spriteCoordinates, 0.0F, playerSpriteScale, WHITE);
                EndDrawing();
            }
            player.spriteCoordinates.x -= 60 / currentObstacleVelocity;
            if (player.spriteCoordinates.x < -20) player1Died = 0;
        }
        if (player2Died)
        {
            framesCounter++;
            if (framesCounter >= (60/framesSpeed))
            {
                framesCounter = 0;
                BeginDrawing();
                    DrawTextureEx(player2Sprite, player2.spriteCoordinates, 0.0F, playerSpriteScale, WHITE);
                EndDrawing();
            }
            player2.spriteCoordinates.x -= 60 / currentObstacleVelocity;
            if (player2.spriteCoordinates.x < -20) player2Died = 0;
        }


        // Collision
        if (player.alive && !player.invincible)
        {

            if (HitObstacle(obstacles, numObstacles, floor, ceiling, player.hitbox))
            {
                player.alive = 0;
                player.speed = 0.0f;
                player1Died = 1;
            }

            if (CheckCollisionRecs(player.hitbox, berry)){
                player2.alive = 1;
                player2.invincible = 1;
                player2.position = player.position;
                berry.x = 1200;
                berryInGame = 0;
                invincibilityTimer = 0.0f;
            }
        }
        if (coop && player2.alive && !player2.invincible)
        {
            if (HitObstacle(obstacles, numObstacles, floor, ceiling, player2.hitbox))
            {
                player2.alive = 0;
                player2.speed = 0.0f;
                player2Died = 1;
            }
            if(CheckCollisionRecs(player2.hitbox, berry)){
                player.alive =1;
                player.invincible = 1;
                player.position = player2.position;
                berry.x = 1200;
                berryInGame= 0;
                invincibilityTimer = 0.0f;
            }
        }

        if (!player.alive)
        {
            if (!player2.alive) break;
        }
    }

    UnloadTexture(playerSprite);
    UnloadTexture(player2Sprite);
    UnloadTexture(obstacleSprite);
    UnloadTexture(berrySprite);

    return score;
}

void UpdatePlayer(Player *player, Player *player2, int coop)
{
    float delta = GetFrameTime();

    // Player 1 movement
    if (player->alive)
    {
        if (IsKeyPressed(KEY_SPACE))
            player->speed = -PLAYER_JUMP_SPD;

        player->position.y += player->speed*delta;
        player->speed += G*delta;
    }

    // Player 2 movement
    if (coop && player2->alive)
    {
        if (IsKeyPressed(KEY_UP))
            player2->speed = -PLAYER_JUMP_SPD;

        player2->position.y += player2->speed*delta;
        player2->speed += G*delta;
    }

}

void UpdateObstacles(Obstacle *obstacles, int *numObstacles, int currentMaxHeightDiff, int upperObstacleHeight, int currentGap, int currentObstacleVelocity)
{
    for (int i = 0; i < *numObstacles; i++)
    {
        Obstacle *obstacle = obstacles + i;
        obstacle->upperRec.x -= 60 / currentObstacleVelocity;
        obstacle->lowerRec.x -= 60 / currentObstacleVelocity;
    }

    CreateNewObstacle(obstacles, numObstacles, currentMaxHeightDiff, upperObstacleHeight, currentGap);
}

void UpdateScore(float *score, Difficulty difficulty, int *currentThreshold, int *currentGap, int *currentMaxHeightDiff, int *currentObstacleVelocity,
    Player player, Player player2, Obstacle obstacles[], Rectangle *berry, int *berryInGame, int coop)
{
    *score += INC_SCORE;

    if (*score > *currentThreshold * difficulty.scoreThreshold)
    {
        *currentGap -= difficulty.gapDecrease;
        *currentMaxHeightDiff += difficulty.maxHeightDiffIncrease;
        *currentObstacleVelocity -= difficulty.obstacleVelocityIncrease;
        *currentThreshold += 1;
        if (coop)
        {
            if(!player.alive || !player2.alive)
                CreateBerry(obstacles, berry, berryInGame, *score);
        }

    }
}

void Draw(Camera2D camera, Obstacle obstacles[], int numObstacles, Texture2D obstacleSprite, Texture2D berrySprite, Player *player, Player *player2, Texture2D playerSprite, Texture2D player2Sprite,
          float playerSpriteScale, float score, int start, int coop, Rectangle berry)
{
    BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginMode2D(camera);
            // Draw obstacles
            for (int i = 0; i < numObstacles; i++)
            {
                Rectangle upper = obstacles[i].upperRec;
                Rectangle lower = obstacles[i].lowerRec;

                // { 2, 0, 24, 139 } are the dimensions of this specific part of the obstacle from the sprite obstacle.png
                DrawTexturePro(obstacleSprite, (Rectangle) { 2, 0, 24, 139 }, (Rectangle) {upper.x, upper.y, upper.width, upper.height}, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
                DrawTexturePro(obstacleSprite, (Rectangle) { 2, 0, 24, -139 }, (Rectangle) {lower.x, lower.y, lower.width, lower.height}, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
                // Obstacle tip height = 40px
                // Tip overflow = 6px
                // Subtract 3 (6 / 2) from the x axis to center the tip of the obstacle
                // { 1, 139, 26, 12 } are the dimensions of this specific part of the obstacle from the sprite obstacle.png
                DrawTexturePro(obstacleSprite, (Rectangle) { 1, 139, 26, 12 }, (Rectangle) {upper.x - 3, upper.height - 40, upper.width + 6, 40}, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
                DrawTexturePro(obstacleSprite, (Rectangle) { 1, 139, 26, 12 }, (Rectangle) {lower.x - 3, lower.y, lower.width + 6, 40}, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
            }
            // Draw Score
            DrawText(TextFormat("Score: %d", (int)score), 0, 0, 40, BLACK);

            if (!start)
                DrawText("Pressione espaco para comecar", 100, 350, 60, BLACK);


            // Draws player

            if (player->alive)
            {
                player->spriteCoordinates = getSpriteCoordinates(*player, playerSpriteScale);
                player->hitbox = getPlayerHitbox(*player, playerSpriteScale);
                DrawTextureEx(playerSprite, player->spriteCoordinates, 0.0F, playerSpriteScale, WHITE);
                //DrawRectangleRec(player->hitbox, WHITE); //Desenha a hitbox
            }

            if (coop && player2->alive)
            {
                player2->spriteCoordinates = getSpriteCoordinates(*player2, playerSpriteScale);
                player2->hitbox = getPlayerHitbox(*player2, playerSpriteScale);
                DrawTextureEx(player2Sprite, player2->spriteCoordinates, 0.0F, playerSpriteScale, WHITE);
                //DrawRectangleRec(player2->hitbox, RED); //Desenha a hitbox
            }

            // Draws Berry
            //DrawRectangleRec(berry, RED);
            DrawTextureEx(berrySprite, (Vector2) {berry.x, berry.y}, 0.0F, 1.0F, WHITE);

    EndDrawing();
}

int HitObstacle(Obstacle obstacles[], int numObstacles, Rectangle floor, Rectangle ceiling, Rectangle playerHitbox)
{
    int hitObstacle = 0;
    if (CheckCollisionRecs(playerHitbox, floor) || CheckCollisionRecs(playerHitbox, ceiling)) hitObstacle = 1;
    if (!hitObstacle)
    {
        for (int i = 0; i < numObstacles; i++)
        {
            if (CheckCollisionRecs(playerHitbox, obstacles[i].upperRec) || CheckCollisionRecs(playerHitbox, obstacles[i].lowerRec))
            {
                hitObstacle = 1;
                break;
            }
        }
    }
    return hitObstacle;
}

void CreateNewObstacle(Obstacle obstacles[], int *numObstacles, int currentMaxHeightDiff, int upperObstacleHeight, int currentGap)
{
    if ((obstacles[*numObstacles - 1].upperRec.x + OBSTACLE_WIDTH) < (1200 - OBSTACLE_DISTANCE_INTERVAL))
    {
        if (*numObstacles == 5)
        {
            // Rotate the array of obstacles. Remove the first element (because it's now out of the screen) and bring each
            // one of them one position down
            for (int i = 1; i < *numObstacles; i++)
                obstacles[i - 1] = obstacles[i];
            *numObstacles -= 1;
        }
        int newLowerObstacleHeight;
        int newUpperObstacleHeight;
        do {
            newUpperObstacleHeight = (-currentMaxHeightDiff) + (rand() % ((currentMaxHeightDiff) - (-currentMaxHeightDiff) + 1)) + upperObstacleHeight;
            newLowerObstacleHeight = 800 - newUpperObstacleHeight -  currentGap;
        } while(newLowerObstacleHeight > 800 - MIN_SIZE_OBS);

        obstacles[*numObstacles] = (Obstacle){(Rectangle){1200, 0, OBSTACLE_WIDTH, newUpperObstacleHeight}, (Rectangle){1200, (800-newLowerObstacleHeight), OBSTACLE_WIDTH, newLowerObstacleHeight}};
        *numObstacles += 1;
    }
}

Vector2 getSpriteCoordinates(Player player, float playerSpriteScale)
{
    return (Vector2) {player.position.x - (20 * (playerSpriteScale - 1)), player.position.y - (18 * (playerSpriteScale - 1))};
}

Rectangle getPlayerHitbox(Player player, float playerSpriteScale)
{
    return (Rectangle) {(player.spriteCoordinates.x + 10), (player.spriteCoordinates.y + 15), 50, 40};
}

void UpdateBerry(Rectangle *berry, int *berryInGame, int currentObstacleVelocity){
    if(*berryInGame){
        berry->x -= 60 / currentObstacleVelocity;
        //if (berry->x < -100) *berryInGame = 0;
    }
}

void CreateBerry(Obstacle obstacles[], Rectangle *berry, int *berryInGame, int score)
{
    berry->height = 100;
    berry->width = 100;
    int xBerry = (100 + obstacles[4].upperRec.x + 140) - (berry->width / 2);


    if (rand()%2 ==1){
        berry->x = xBerry;
        berry->y = 200;
    } else {
        berry->x = xBerry;
        berry->y = 600;
    }
    *berryInGame = 1;
}

