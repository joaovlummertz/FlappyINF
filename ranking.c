#include "ranking.h"

#define LIGHTPINK    (Color){ 239, 223, 245, 255 }
#define DARKPINK    (Color){ 245, 84, 134, 255 }
#define DARKDARKPINK    (Color){ 245, 38, 134, 255 }
#define LILAC    (Color){ 181, 176, 245, 255 }
#define DARKLILAC    (Color){ 181, 88, 245, 255 }
#define DARKDARKLILAC    (Color){ 181, 33, 245, 255 }
#define PINK    (Color){ 245, 171, 239, 255 }


void LoadRanking(Ranking *rankingData)
{
    FILE *archivePointer;
    archivePointer = fopen("ranking.bin", "rb");

    if (archivePointer != NULL)
    {
        fread(rankingData, sizeof(Ranking), 5, archivePointer);
        fclose(archivePointer);
    } else printf("Erro na abertura do arquivo");
}

bool NewHighScore(Ranking ranking[]){
    bool isNewHighScore=0;
    if(ranking[5].scoreRank > ranking[4].scoreRank){
        isNewHighScore =1;
    }
    return isNewHighScore;
}

void OrganizeHighScores(Ranking *ranking){
    int indice;
    int tempScore;
    char tempName[4];

    for(indice=4; indice>=0; indice--){
        if (ranking[indice+1].scoreRank > ranking[indice].scoreRank){
            tempScore = ranking[indice].scoreRank;
            strcpy(tempName, ranking[indice].nameRank);
            ranking[indice].scoreRank = ranking[indice+1].scoreRank;
            strcpy(ranking[indice].nameRank, ranking[indice+1].nameRank);
            ranking[indice+1].scoreRank = tempScore;
            strcpy(ranking[indice+1].nameRank, tempName);

        }

    }
}

void RewriteRankingArchive(Ranking ranking[]){
    FILE *archivePointer;

    archivePointer = fopen("ranking.bin", "wb");

    if(archivePointer != NULL){
        fwrite(ranking, sizeof(ranking[0]), 5, archivePointer);

    } else printf("Erro ao abrir arquivo!");

    fclose(archivePointer);

}

void GetNameInputAndDrawEnterName(Ranking *ranking){
    char nameInput[4];
    int letterIndice = 0;

    while (!IsKeyPressed(KEY_ENTER) || letterIndice < 3){
        fflush(stdin);
        int key = GetCharPressed();

        BeginDrawing();
        ClearBackground(LIGHTPINK);
        DrawText("High score! Entre com o seu nome", 180, 180, 50, DARKDARKPINK);
        DrawRectangle(400, 300, 350, 150, LILAC);
        if (letterIndice!=0)
            DrawText(nameInput, 470, 330, 100, BLACK);
        EndDrawing();

        while (key > 64)
        {
            if ((key >= 65) && (key <= 122) && (letterIndice < 3))
            {
                nameInput[letterIndice] = (char)key;
                nameInput[letterIndice+1] = '\0'; // Add null terminator at the end of the string.
                letterIndice++;
            }

            fflush(stdin);
            key = GetCharPressed();  // Check next character in the queue
         }

         if (IsKeyPressed(KEY_BACKSPACE))
         {
            letterIndice--;
            if (letterIndice < 0) letterIndice = 0;
            nameInput[letterIndice] = '\0';
         }

    }

    strcpy(ranking[5].nameRank, nameInput);
    printf("String salva no array!");

}




