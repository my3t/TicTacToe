// TODO: validation for input
// TODO: game type specific modules in seperate files
// TODO: spruce up GUI

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#include "include/variables/global.c"

#include "include/functions/win.c"
#include "include/ai/minimax.c"
#include "include/functions/logic.c"

#include "include/gui/board.c"
#include "include/gui/mainmenu.c"

void drawMenu();
void drawBoard(const char board[9]);
void drawEllipse(int x, int y, int rx, int ry);
void clearScreen();
void playerMove(char symbol, char board[9]);
void computerMove(char board[9]);
int minimax(char board[9], char player, int depth, int alpha, int beta);
char win(const char board[9]);

int main(int argc, char *argv[])
{
    char board[9] = {'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b'}; // computer squares are x, player squares are o, empty squares are b
    int gameType;
    int game = 1;
    int turn = 0;
    char player = O_SYMBOL;

    // TODO: make into window intialisation into function
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() != 0)
    {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    // Create renderer
    _renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (_renderer == NULL)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    _font = TTF_OpenFont("fonts/pcsenior.ttf", 24);
    if (_font == NULL)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Event event;
    // TODO: if can afford, make this more modular (split into functions)
    // DONT SPLIT INTO FUNCTIONS UNTIL DESIGN IS FINALISED
    while (game)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return 1;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                if (y >= 150 && y < 250)
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50)
                    {
                        drawBoard(board);
                        // PVP clicked
                        printf("PVP clicked\n");
                        gameType = TWO_PLAYER_GAME;
                        if (gameType == TWO_PLAYER_GAME)
                        { // check game type
                            for (turn = 0; turn < 9 && win(board) == EMPTY_SYMBOL; ++turn)
                            { // loop until turns finish or there is no win
                                if (player == X_SYMBOL)
                                {                                // check current player type
                                    playerMove(X_SYMBOL, board); // do move
                                    player = O_SYMBOL;           // switch to other player
                                }
                                else
                                {
                                    playerMove(O_SYMBOL, board);
                                    player = X_SYMBOL;
                                }

                                drawBoard(board); // redraw board after changes
                            }

                            char winner = win(board); // check winner once got winner or out of turns

                            if (winner == X_SYMBOL)
                            {                                                                                                             // check winner type
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 1 (X) is the winner!", window); // alert user on winner
                            }
                            else if (winner == O_SYMBOL)
                            {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Player 2 (O) is the winner!", window);
                            }
                            else
                            {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "The game ended in a draw...", window);
                            }
                        }
                    }
                }
                else if (y >= 250 && y < 300) // PVAI button
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 2)
                    {
                        drawBoard(board);
                        // PVAI clicked
                        printf("PVAI clicked\n");
                        gameType = MINIMAX_GAME;
                        if (gameType == MINIMAX_GAME)
                        {
                            playPvAI(board, _renderer, window);
                        }
                    }
                }
                else if (y >= 300 && y < 400)
                {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "AI Mode", "Mode is still under construction!", window);
                    //     game = 0;
                }
                else
                {
                    if (x >= SCREEN_WIDTH / 2 - 50 && x < SCREEN_WIDTH / 2 + 50 * 3)
                    {
                        // Exit clicked
                        printf("Exit clicked\n");
                        return 1;
                    }
                }
            }
        }
        // else
        // {
        //     SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "AI Mode", "Mode is still under construction!", window);
        //     game = 0;
        // }

        // reset game
        player = O_SYMBOL;
        for (int i = 0; i < 9; ++i)
        {
            board[i] = 'b';
        }

        // Draw the menu
        drawMenu();

        // Present the renderer
        SDL_RenderPresent(_renderer);
    }

    // TTF_CloseFont(_font);
    TTF_Quit();
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}