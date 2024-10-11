/* Menu 14/02/24
primeiro teste janela menu

FLAPPY BIRD INF

*/

#include "game.h"
#include "difficulty.h"
#include "ranking.h"
#include <stdio.h>

//define cores dos objetos
#define LIGHTPINK    (Color){ 239, 223, 245, 255 }
#define DARKPINK    (Color){ 245, 84, 134, 255 }
#define DARKDARKPINK    (Color){ 245, 38, 134, 255 }
#define LILAC    (Color){ 181, 176, 245, 255 }
#define DARKLILAC    (Color){ 181, 88, 245, 255 }
#define DARKDARKLILAC    (Color){ 181, 33, 245, 255 }
#define PINK    (Color){ 245, 171, 239, 255 }


typedef struct ButtonsCoords {
    Rectangle FirstButton;
    Rectangle SecondButton;
    Rectangle ThirdButton;
    Rectangle FourthButton;
}STRUCTBUTTONS;



//funcao chamada por main
int UpdateMenu(bool inMainMenu, int currentDifficulty);

//função chamada por UpdateMenu
void DrawMainMenu(int buttonHover, int currentDifficulty);

//funcao chamada por UpdateMenu
void DrawDifficultyMenu(int buttonHover);

//função chamada por main
bool DrawRankingMenu(Ranking rankingData[]);

//funcao chamada por main
int DrawGameOver(void);

int SingleMultiPlayerMenu(void);

int main(void)
{
    //definicao de variaveis de posição do mouse
    bool inMainMenu = 1;
    int button = 0;
    Difficulty currentDifficulty = LoadDifficulty(2); // Default difficulty is medium
    int isMultiplayer;

    //variaveis ranking
    Ranking rankingData[6];
    bool newHighScore;
    int score;

    // Loads ranking data from ranking.bin file
    LoadRanking(rankingData);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "FlappyINF");

    //coloca num de fps maximo
    SetTargetFPS(60);

    //inicia código principal
    while (!WindowShouldClose())
    {
       //desenha menu se estiver no menu
       if (inMainMenu)
       {
          button = UpdateMenu(inMainMenu, currentDifficulty.difficultyLevel);  //1 se botao de inicio, 2 se ranking, etc
          //se botao foi apertado sai do menu e abre a tela desejada
          if (button != 0)
              inMainMenu = 0;
       }
       else
       {
            if(button ==1){
                isMultiplayer = SingleMultiPlayerMenu();
                if(isMultiplayer){
                    rankingData[5].scoreRank = play(currentDifficulty, 1);
                }
                else{
                    rankingData[5].scoreRank = play(currentDifficulty, 0);
                    newHighScore = NewHighScore(rankingData);
                    if(newHighScore){
                        GetNameInputAndDrawEnterName(rankingData);
                        OrganizeHighScores(rankingData);
                        RewriteRankingArchive(rankingData);
                    }
                }
                inMainMenu = DrawGameOver();
                if(inMainMenu == 1)
                    DrawMainMenu(0, currentDifficulty.difficultyLevel);
            } else if (button == 2){

                int clickedDifficulty = UpdateMenu(inMainMenu, currentDifficulty.difficultyLevel);
                if (clickedDifficulty != 0)
                {
                    if (clickedDifficulty != 4) currentDifficulty = LoadDifficulty(clickedDifficulty);
                    inMainMenu = 1;
                }
            } else if(button ==3){
                // Loads ranking data from ranking.bin file
                LoadRanking(rankingData);
                inMainMenu = DrawRankingMenu(rankingData);
            } else if(button ==4){
                break;
            }
       }
    }

    CloseWindow();

    return 0;
}

int SingleMultiPlayerMenu(void){
    Vector2 mouseCoord;
    bool mouseClick = 0;
    int buttonPressed = -1;
    Rectangle buttons[2] = {
        (Rectangle) {430, 235, 380, 140},
        (Rectangle) {430, 425, 380, 140}
    };

    while(!mouseClick || buttonPressed == -1){
        mouseCoord = GetMousePosition();
        mouseClick = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        BeginDrawing();

        ClearBackground(LIGHTPINK);
        DrawText("Escolha o modo de jogo", 300, 100, 55, BLACK);

        if (CheckCollisionPointRec(mouseCoord, buttons[0])){
            DrawRectangle(430, 235, 380, 140, DARKLILAC);
            DrawRectangle(430, 425, 380, 140, DARKPINK);
            if(mouseClick)
                buttonPressed = 0;

        } else if(CheckCollisionPointRec(mouseCoord, buttons[1])){
            DrawRectangle(430, 235, 380, 140, LILAC);
            DrawRectangle(430, 425, 380, 140, DARKDARKPINK);
            DrawText("Esse modo nao sera contabilizado no ranking", 315, 650, 30, BLACK);
            if(mouseClick)
                buttonPressed = 1;

        } else {
            DrawRectangle(430, 235, 380, 140, LILAC);
            DrawRectangle(430, 425, 380, 140, DARKPINK);
        }

        DrawText("UM JOGADOR", 480, 280, 45, BLACK);
        DrawText("DOIS JOGADORES", 455, 480, 38, BLACK);

        EndDrawing();
    }

    return buttonPressed;
}


int UpdateMenu(bool inMainMenu, int currentDifficulty)
{
    Vector2 mouseCoord;
    bool mouseClick = 0;
    int button = 0;

    STRUCTBUTTONS buttonscoord = (STRUCTBUTTONS){
        (Rectangle){495, 260, 240, 80},
        (Rectangle){495, 380, 240, 80},
        (Rectangle){495, 500, 240, 80},
        (Rectangle){495, 620, 240, 80},
    };

    //pega a posição do mouse
    mouseCoord = GetMousePosition();
    mouseClick = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    //MOUSE NO PRIMEIRO BOTAO
    if (CheckCollisionPointRec(mouseCoord, buttonscoord.FirstButton))
    {
        if (inMainMenu)
            DrawMainMenu(1, currentDifficulty);
        else
            DrawDifficultyMenu(1);
        if (mouseClick)
            button = 1;
    }
    //MOUSE NO SEGUNDO BOTAO
    else if (CheckCollisionPointRec(mouseCoord, buttonscoord.SecondButton))
    {
        if (inMainMenu)
            DrawMainMenu(2, currentDifficulty);
        else
            DrawDifficultyMenu(2);
        if (mouseClick)
            button = 2;
    }
    //MOUSE NO TERCEIRO BOTAO
    else if (CheckCollisionPointRec(mouseCoord, buttonscoord.ThirdButton))
    {
        if (inMainMenu)
            DrawMainMenu(3, currentDifficulty);
        else
            DrawDifficultyMenu(3);
        if (mouseClick)
            button = 3;
    }
    //MOUSE NO QUARTO BOTAO
    else if (CheckCollisionPointRec(mouseCoord, buttonscoord.FourthButton))
    {
        if (inMainMenu)
            DrawMainMenu(4, currentDifficulty);
        else
            DrawDifficultyMenu(4);
        if (mouseClick)
            button = 4;
    }
    //MOUSE EM NENHUM BOTAO
    else
    {
        if (inMainMenu)
            DrawMainMenu(0, currentDifficulty);
        else
            DrawDifficultyMenu(0);
        button = 0;
    }

    return button;
}

void DrawMainMenu(int buttonHover, int currentDifficulty)
{
    BeginDrawing();
        //define cor do background
        ClearBackground(LIGHTPINK);

        //desenha texto de título
        DrawText("FLAPPY INF", 360, 90, 80, BLACK);

        //sei que esse bloco aqui fica feio mas os mil if else eram mais feios :/
        switch (buttonHover){
            case 1:
                DrawRectangle(495, 260, 240, 80, DARKDARKPINK);
                DrawText(TextFormat("inicia o jogo com a dificuldade %s, para mudar, selecione dificuldade", difficultyToString(currentDifficulty)), 90, 730, 30, DARKDARKPINK);
                DrawRectangle(495, 380, 240, 80, LILAC);
                DrawRectangle(495, 500, 240, 80, LILAC);
                DrawRectangle(495, 620, 240, 80, PINK);
                break;
            case 2:
                DrawRectangle(495, 380, 240, 80, DARKLILAC);
                DrawText("seleciona a dificuldade do jogo", 390, 730, 30, DARKDARKPINK);
                DrawRectangle(495, 260, 240, 80, DARKPINK);
                DrawRectangle(495, 500, 240, 80, LILAC);
                DrawRectangle(495, 620, 240, 80, PINK);
                break;
            case 3:
                DrawRectangle(495, 500, 240, 80, DARKLILAC);
                DrawText("mostra o ranking atual", 430, 730, 30, DARKDARKPINK);
                DrawRectangle(495, 260, 240, 80, DARKPINK);
                DrawRectangle(495, 380, 240, 80, LILAC);
                DrawRectangle(495, 620, 240, 80, PINK);
                break;
            case 4:
                DrawRectangle(495, 620, 240, 80, DARKPINK);
                DrawText("sai do jogo", 540, 730, 30, DARKDARKPINK);
                DrawRectangle(495, 260, 240, 80, DARKPINK);
                DrawRectangle(495, 380, 240, 80, LILAC);
                DrawRectangle(495, 500, 240, 80, LILAC);
                break;
            case 0:
                DrawRectangle(495, 260, 240, 80, DARKPINK);
                DrawRectangle(495, 380, 240, 80, LILAC);
                DrawRectangle(495, 500, 240, 80, LILAC);
                DrawRectangle(495, 620, 240, 80, PINK);
                break;
        }

        //desenha texto dos botões
        DrawText("JOGAR", 565, 285, 30, BLACK);
        DrawText("DIFICULDADE", 515, 405, 30, BLACK);
        DrawText("RANKING", 550, 525, 30, BLACK);
        DrawText("SAIR", 580, 645, 30, BLACK);

    EndDrawing();
}

void DrawDifficultyMenu(int buttonHover)
{
    BeginDrawing();

        ClearBackground(LIGHTPINK);

        DrawText("DIFICULDADE", 325, 90, 80, BLACK);

        DrawRectangle(495, 260, 240, 80, LILAC);
        DrawRectangle(495, 380, 240, 80, DARKLILAC);
        DrawRectangle(495, 500, 240, 80, DARKPINK);
        DrawRectangle(495, 620, 240, 80, PINK);

        switch (buttonHover) {
        case 1:
            DrawRectangle(495, 260, 240, 80, DARKLILAC);
            DrawText("seleciona a dificuldade facil", 390, 730, 30, DARKDARKPINK);
            break;
        case 2:
            DrawRectangle(495, 380, 240, 80, DARKDARKLILAC);
            DrawText("seleciona a dificuldade media", 390, 730, 30, DARKDARKPINK);
            break;
        case 3:
            DrawRectangle(495, 500, 240, 80, DARKDARKPINK);
            DrawText("seleciona a dificuldade dificil", 390, 730, 30, DARKDARKPINK);
            break;
        case 4:
            DrawRectangle(495, 620, 240, 80, DARKDARKPINK);
            DrawText("volta para o menu inicial", 440, 730, 30, DARKDARKPINK);
            break;
        default:
            break;
        }

        DrawText("FACIL", 570, 285, 30, BLACK);
        DrawText("MEDIO", 565, 405, 30, BLACK);
        DrawText("DIFICIL", 560, 525, 30, BLACK);
        DrawText("VOLTAR", 555, 645, 30, BLACK);

    EndDrawing();
}

bool DrawRankingMenu(Ranking rankingData[])
{
    Vector2 mouseCoord = GetMousePosition();
    Rectangle buttonExit = {500, 660, 200, 70};
    bool inMainMenu = 0;
    int text_size[5];
    int indice;

    for(indice=0; indice<5; indice++){
        text_size[indice] = MeasureText(TextFormat("%s", rankingData[indice].nameRank), 35);
    }

    BeginDrawing();

        ClearBackground(LIGHTPINK);

        DrawText("RANKING", 380, 90, 90, BLACK);

        DrawRectangle(470, 250, 130, 60, LILAC);
        DrawRectangle(620, 250, 150, 60, LILAC);

        DrawRectangle(470, 330, 130, 60, LILAC);
        DrawRectangle(620, 330, 150, 60, LILAC);

        DrawRectangle(470, 410, 130, 60, LILAC);
        DrawRectangle(620, 410, 150, 60, LILAC);

        DrawRectangle(470, 490, 130, 60, LILAC);
        DrawRectangle(620, 490, 150, 60, LILAC);

        DrawRectangle(470, 570, 130, 60, LILAC);
        DrawRectangle(620, 570, 150, 60, LILAC);


        if(CheckCollisionPointRec(mouseCoord, buttonExit)){
            DrawRectangle(500, 660, 200, 70, DARKDARKPINK);
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                inMainMenu =1;
        }
        else
            DrawRectangle(500, 660, 200, 70, DARKPINK);

        DrawText("1-", 430, 260, 40, DARKDARKPINK);
        DrawText("2-", 420, 340, 40, DARKDARKPINK);
        DrawText("3-", 420, 420, 40, DARKDARKPINK);
        DrawText("4-", 420, 500, 40, DARKDARKPINK);
        DrawText("5-", 420, 580, 40, DARKDARKPINK);

        DrawText(TextFormat("%s", rankingData[0].nameRank), (470+((130 -text_size[0])/2)), 265, 35, BLACK);
        DrawText(TextFormat("%s", rankingData[1].nameRank), (470+((130 -text_size[1])/2)), 345, 35, BLACK);
        DrawText(TextFormat("%s", rankingData[2].nameRank), (470+((130 -text_size[2])/2)), 425, 35, BLACK);
        DrawText(TextFormat("%s", rankingData[3].nameRank), (470+((130 -text_size[3])/2)), 505, 35, BLACK);
        DrawText(TextFormat("%s", rankingData[4].nameRank), (470+((130 -text_size[4])/2)), 585, 35, BLACK);

        DrawText(TextFormat("%d", rankingData[0].scoreRank), 630, 265, 30, BLACK);
        DrawText(TextFormat("%d", rankingData[1].scoreRank), 630, 345, 30, BLACK);
        DrawText(TextFormat("%d", rankingData[2].scoreRank), 630, 425, 30, BLACK);
        DrawText(TextFormat("%d", rankingData[3].scoreRank), 630, 505, 30, BLACK);
        DrawText(TextFormat("%d", rankingData[4].scoreRank), 630, 585, 30, BLACK);

        DrawText("NOME", 498, 220, 28, DARKDARKPINK);
        DrawText("SCORE",645, 220, 28, DARKDARKPINK);

        DrawText("VOLTAR", 537, 680, 30, BLACK);

    EndDrawing();

    return inMainMenu;
}

int DrawGameOver(void){
    Vector2 mouseCoord;
    Rectangle buttonOne = {495, 380, 240, 80};
    Rectangle buttonTwo = {495, 500, 240, 80};
    bool mouseClick = 0;
    bool buttonpressed = -1;

    while(!mouseClick || buttonpressed == -1){

    mouseCoord = GetMousePosition();
    mouseClick = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    BeginDrawing();

    ClearBackground(LIGHTPINK);
    DrawText("GAME OVER", 250, 120, 120, BLACK);
    if(CheckCollisionPointRec(mouseCoord, buttonOne)){
        DrawRectangle(495, 380, 240, 80, DARKDARKLILAC);
        DrawRectangle(495, 500, 240, 80, DARKPINK);
        DrawText("Recomeca o jogo", 515, 730, 30, DARKDARKPINK);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            buttonpressed = 0;
    }
    else if(CheckCollisionPointRec(mouseCoord, buttonTwo)){
        DrawRectangle(495, 500, 240, 80, DARKDARKPINK);
        DrawRectangle(495, 380, 240, 80, DARKLILAC);
        DrawText("Volta para o menu principal", 410, 730, 30, DARKDARKPINK);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            buttonpressed = 1;
    }else{
        DrawRectangle(495, 380, 240, 80, DARKLILAC);
        DrawRectangle(495, 500, 240, 80, DARKPINK);
    }

    DrawText("REINICIAR", 533, 405, 30, BLACK);
    DrawText("VOLTAR", 550, 525, 30, BLACK);

    EndDrawing();

    }

    return buttonpressed;

}




