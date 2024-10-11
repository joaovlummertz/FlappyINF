
#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"

#ifndef RANKING_INCLUDED
#define RANKING_INCLUDED

typedef struct Ranking {
    int scoreRank;
    char nameRank[4];

} Ranking;

//funcoes ranking
//funcao chamada por main
bool NewHighScore(Ranking ranking[]);

//função chamada por main
void LoadRanking(Ranking *rankingData);

//funcao chamada por main
void OrganizeHighScores(Ranking *ranking);

//funcao chamada por main
void RewriteRankingArchive(Ranking ranking[]);

//funcao chamada por main
void GetNameInputAndDrawEnterName(Ranking *ranking);


#endif // RANKING_INCLUDED
