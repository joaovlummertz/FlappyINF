#ifndef DIFFICULTY_H_INCLUDED
#define DIFFICULTY_H_INCLUDED

typedef enum DifficultyLevel
{
    EASY = 1,
    MEDIUM,
    HARD
} DifficultyLevel;

typedef struct Difficulty
{
    DifficultyLevel difficultyLevel;
    int scoreThreshold;
    int gap;
    int gapDecrease;
    int maxHeigtDiff;
    int maxHeightDiffIncrease;
    int obstacleVelocity;
    int obstacleVelocityIncrease;
} Difficulty;

Difficulty LoadDifficulty(DifficultyLevel level);

char * difficultyToString(DifficultyLevel level);

#endif // DIFFICULTY_H_INCLUDED
