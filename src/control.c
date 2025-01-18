#include "../control.h"

bool control(){
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Event evento;

    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            return false;
        }
    }       

    if (state[SDL_SCANCODE_UP]) {
        posY += PLAYER_SPEED;
    }if (state[SDL_SCANCODE_DOWN]) {
        posY -= PLAYER_SPEED;
    }if (state[SDL_SCANCODE_LEFT]) {
        playerAngle -= 0.1;
    }if (state[SDL_SCANCODE_RIGHT]) {
        playerAngle += 0.1;
    }
    
    return true;
}