#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "raylib.h"
#include "raymath.h"
#include "difficulty.h"
#include <stdio.h>
#include <time.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

typedef struct Player
{
    Vector2 position;
    float speed;
    Vector2 spriteCoordinates;
    Rectangle hitbox;
    int alive;
    int invincible;
} Player;

typedef struct Obstacle
{
    Rectangle upperRec;
    Rectangle lowerRec;
} Obstacle;

void UpdatePlayer(Player *player, Player *player2, int coop);

void UpdateObstacles(Obstacle *obstacles, int *numObstacles, int currentMaxHeightDiff, int upperObstacleHeight, int currentGap, int currentObstacleVelocity);

void UpdateScore(float *score, Difficulty difficulty, int *currentThreshold, int *currentGap, int *currentMaxHeightDiff, int *currentObstacleVelocity,
    Player player, Player player2, Obstacle obstacles[], Rectangle *berry, int *berryInGame, int coop);

void Draw(Camera2D camera, Obstacle obstacles[], int numObstacles, Texture2D obstacleSprite, Texture2D berrySprite, Player *player, Player *player2, Texture2D playerSprite, Texture2D player2Sprite,
          float playerSpriteScale, float score, int start, int coop, Rectangle berry);

int HitObstacle(Obstacle obstacles[], int numObstacles, Rectangle floor, Rectangle ceiling, Rectangle playerHitbox);

void CreateNewObstacle(Obstacle obstacles[], int *numObstacles, int currentMaxHeightDiff, int upperObstacleHeight, int currentGap);

int play(Difficulty difficulty, int coop);

void UpdateBerry(Rectangle *berry, int *berryInGame, int currentObstacleVelocity);

void CreateBerry(Obstacle obstacles[], Rectangle *berry, int *berryInGame, int score);

#endif // GAME_H_INCLUDED
