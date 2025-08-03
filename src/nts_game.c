#include <SDL2/SDL_image.h>
#include "time.h"

#include "nts_game.h"


void LoadGame(GameState *game, int screen_width, int screen_height, SDL_Window *window, Uint32 flags)
{   
    srand(time(NULL));

    game->elapsed_time = 0.0f;
    game->delta_time = 0.0f;
    
    game->renderer = SDL_CreateRenderer(window, -1, flags);
    if (game->renderer == NULL)
    {
        printf("ERROR::ENGINE::LOAD - Could not create renderer\n");
        QuitGame(game, 1);
    }

    game->SCREEN_WIDTH = screen_width;
    game->SCREEN_HEIGHT = screen_height;
    
    game->gameobjects = NULL;
    game->nb_gameobjects = 0;

    game->sprites = NULL;
    game->nb_sprites = 0;

    LoadTexture(game, (char *)"resources/images/default.png");
}


// Render gameobjects
void doRender(GameState *game)
{
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);
    SDL_RenderClear(game->renderer);

    for (int i = 0; i < game->nb_gameobjects; i++)
    {
        if (game->gameobjects[i].sprite_renderer != NULL)
        {
            SDL_Rect rect = {
                (int)game->gameobjects[i].position.x,
                (int)game->gameobjects[i].position.y,
                (int)(game->gameobjects[i].sprite_renderer->width * game->gameobjects[i].scale.x),
                (int)(game->gameobjects[i].sprite_renderer->height * game->gameobjects[i].scale.y)
            };
            SDL_RenderCopy(game->renderer, game->gameobjects[i].sprite_renderer->texture, NULL, &rect);
        }
    }
    SDL_RenderPresent(game->renderer);
}

// Update elasped time and delta time
void updateTime(GameState *game)
{
    float last_time = game->elapsed_time;
    game->elapsed_time = SDL_GetTicks();
    game->delta_time = (game->elapsed_time - last_time) / 1000.0f;
}


// Load usable texture in the engine
Sprite *LoadTexture(GameState *game, char *file_location)
{

    for (int i = 0; i < game->nb_sprites; i++) {
        if (strcmp(game->sprites[i].file_name, file_location) == 0) {
            return &game->sprites[i]; // reuse existing
        }
    }

    SDL_Surface *surface = IMG_Load(file_location);
    if (surface == NULL)
    {
        printf("ERROR: Could not load file '%s'\n", file_location);
        QuitGame(game, 1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    if (texture == NULL)
    {
        printf("ERROR: Could not create texture from '%s'\n", file_location);
        QuitGame(game, 1);
    }
    
    Sprite *temp = realloc(game->sprites, (game->nb_sprites + 1) * sizeof(Sprite));
    if (temp == NULL)
    {
        SDL_DestroyTexture(texture);
        printf("ERROR: Could not reallocate sprite array\n");
        QuitGame(game, 1);
    }
    
    game->sprites = temp;
    
    // Specifying values for 
    Sprite *sprite = &game->sprites[game->nb_sprites];
    char *uuid = gen_uuid();
    sprite->uuid = malloc(strlen(uuid) + 1); // +1 for null terminator
    if (sprite->uuid == NULL)
    {
        printf("ERROR::TEXTURE::LOAD - allocation error on char*\n");
        QuitGame(game, 1);
    }
    strcpy(sprite->uuid, uuid);
    free(uuid);
    
    sprite->file_name = malloc(strlen(file_location) + 1);
    if (sprite->file_name == NULL)
    {   
        printf("ERROR::TEXTURE::LOAD - allocation error on char*\n");
        QuitGame(game, 1);
    }
    strcpy(sprite->file_name, file_location);
    
    sprite->texture = texture;
    sprite->width = surface->w;
    sprite->height = surface->h;
    

    SDL_FreeSurface(surface);
    game->nb_sprites += 1;

    return sprite;
}

// Add a new gameobject in a GameState
GameObject *CreateGameObject(GameState *game, char *name, int sprite_index)
{
    GameObject *temp_gameobjects = realloc(game->gameobjects, (game->nb_gameobjects + 1) * sizeof(GameObject));
    if (temp_gameobjects == NULL)
    {
        printf("ERROR::GAMEOBJECT::CREATE - Could not realloc gameobject array\n");
        QuitGame(game, 1);
    }

    game->gameobjects = temp_gameobjects;

    // Updating the new game object
    GameObject *new_gameobject = &game->gameobjects[game->nb_gameobjects];

    new_gameobject->name = malloc((strlen(name) + 1));
    if (new_gameobject->name == NULL)
    {
        printf("ERROR::GAMEOBJECT::CREATE - allocation error on char*\n");
        QuitGame(game, 1);
    }
    strcpy(new_gameobject->name, name);

    char *uuid = gen_uuid();
    new_gameobject->uuid = malloc((strlen(uuid) + 1));
    if (new_gameobject->uuid == NULL)
    {
        printf("ERROR::GAMEOBJECT::CREATE - allocation error on char*\n");
        QuitGame(game, 1);
    }
    strcpy(new_gameobject->uuid, uuid);
    free(uuid);
    new_gameobject->scale = (fVec2) {1.0f, 1.0f};;
    new_gameobject->position = (fVec2) {0.0f, 0.0f};

    if (sprite_index >= game->nb_sprites) // attributing 
    {
        printf("ERROR::GAMEOBJECT::CREATE - Sprite index too high");
        QuitGame(game, 1);
    }
    else if (sprite_index < 0)
    {
        new_gameobject->sprite_renderer = NULL;
    } else 
    {
        new_gameobject->sprite_renderer = &game->sprites[sprite_index];
    }

    game->nb_gameobjects += 1;

    return new_gameobject;
}

void QuitGame(GameState *game, int caused_by_error)
{
    for (int i = 0; i < game->nb_sprites; i++)
    {
        SDL_DestroyTexture(game->sprites[i].texture);
        free(game->sprites[i].file_name);
        free(game->sprites[i].uuid);
    }

    for (int i = 0; i < game->nb_gameobjects; i++)
    {
        free(game->gameobjects[i].name);
        free(game->gameobjects[i].uuid);
    }

    free(game->sprites);
    free(game->gameobjects);

    SDL_DestroyRenderer(game->renderer);
    SDL_Quit();
    if (caused_by_error)
    {
        exit(1);
    }
}

char* gen_uuid() {
    char v[] = "0123456789abcdef";
    char* buf = malloc(37);
    if (!buf) return NULL;

    for (int i = 0; i < 36; ++i) {
        buf[i] = v[rand() % 16];
    }
    buf[8] = buf[13] = buf[18] = buf[23] = '-';
    buf[36] = '\0';
    return buf;
}
