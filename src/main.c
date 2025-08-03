#include <stdio.h>
#include <SDL2/SDL_image.h>

#include "nts_game.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


int main(int argc, char const *argv[])
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        exit(1);
    }

    GameState game;
    SDL_Window *window = NULL;
    game.renderer = NULL;

    window = SDL_CreateWindow("Game Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );
    
    LoadGame(&game, SCREEN_WIDTH, SCREEN_HEIGHT, window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    CreateGameObject(&game, "test", 0);
    game.gameobjects[0].scale = (fVec2) {5.0f, 2.0f};
    doRender(&game);

    SDL_Delay(3000);

    QuitGame(&game, 0);
    return 0;
}
