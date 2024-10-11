#include "difficulty.h"
#include <stdio.h>

Difficulty LoadDifficulty(DifficultyLevel level)
{
    Difficulty difficulty;
    difficulty.difficultyLevel = level;

    FILE *file;
    if (level == 1)
        file = fopen("files/difficulty/easy.txt", "r");
    else if (level == 2)
        file = fopen("files/difficulty/medium.txt", "r");
    else if (level == 3)
        file = fopen("files/difficulty/hard.txt", "r");

    if (file != NULL)
    {
        fscanf(file, "%d %d %d %d %d %d %d",
              &difficulty.scoreThreshold,
              &difficulty.gap,
              &difficulty.gapDecrease,
              &difficulty.maxHeigtDiff,
              &difficulty.maxHeightDiffIncrease,
              &difficulty.obstacleVelocity,
              &difficulty.obstacleVelocityIncrease);
    }
    else printf("Erro na leitura do arquivo de dificuldade!\n");

    fclose(file);

    return difficulty;
}

char * difficultyToString(DifficultyLevel level)
{
    switch (level) {
    case 1: return "facil";
    case 2: return "media";
    case 3: return "dificil";
    default: return "";
    }
}
