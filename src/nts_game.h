#ifndef NTS_GAME_H
#define NTS_GAME_H

#include <SDL2/SDL.h>
#include "nts_math.h"
#include "nts_entity.h"


typedef struct {
    char *uuid;
    char *file_name;  
    SDL_Texture *texture;
    int width;
    int height;
    
} Sprite;

typedef struct {

    char *name;
    char *uuid;
    
    fVec2 position;
    fVec2 scale;

    Sprite *sprite_renderer;

} GameObject;


typedef struct {

    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    
    float elapsed_time;
    float delta_time;

    SDL_Renderer *renderer;
    Sprite *sprites;
    size_t nb_sprites;

    GameObject *gameobjects;
    size_t nb_gameobjects;
    
} GameState;

void LoadGame(GameState *game, int screen_width, int screen_height, SDL_Window *window, Uint32 flags);

void doRender(GameState *game);

void updateTime(GameState *game);

Sprite *LoadTexture(GameState *game, char *file_location);

void UnloadTexture(GameState *game, Sprite *sprite_to_delete);

GameObject *CreateGameObject(GameState *game, char *name, int sprite_index);

void QuitGame(GameState *game, int caused_by_error);

char* gen_uuid();

#endif