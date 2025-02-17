#include "index.h"
#include "control.h"


#define MAP_WIDTH 13
#define MAP_HEIGHT 8

// Mapa do jogo (1 = parede, 0 = espaço vazio)
int worldMap[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,1,1,1,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Variáveis do jogador
double posX = 4, posY = 4;  // Posição inicial
double dirX, dirY;
double planeX = 0, planeY = 0.66; // Plano da câmera
double playerAngle = 0;

int initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("3D-RayCaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Erro ao criar renderizador: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

// Função que desenha as paredes com Raycasting
void renderScene(SDL_Renderer *renderer) {
    // Pintando o céu
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_Rect skyRect = {0, 0, WIDTH, HEIGHT / 2};
    SDL_RenderFillRect(renderer, &skyRect);
    
    // Pintando o chão
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_Rect floorRect = {0, HEIGHT / 2, WIDTH, HEIGHT / 2};
    SDL_RenderFillRect(renderer, &floorRect);

    for (int x = 0; x < WIDTH; x++) {
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        double sideDistX, sideDistY;
        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX, stepY, hit = 0, side;

        // Determinar a direção do raio
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        // Algoritmo DDA (Digital Differential Analyzer)
        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (worldMap[mapY][mapX] > 0) hit = 1;
        }

        // Distância perpendicular à parede
        if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        // Altura da linha da parede
        int lineHeight = (int)(HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        // Cor diferente dependendo do lado atingido (simulação de sombra)
        if (side == 1) SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }

    SDL_RenderPresent(renderer);
}

// Atualiza os vetores da câmera
void updateCamera() {
    dirX = cos(playerAngle);
    dirY = sin(playerAngle);
    planeX = -0.66 * sin(playerAngle);
    planeY = 0.66 * cos(playerAngle);
}

// Loop principal do jogo
void gameLoop(SDL_Renderer *renderer) {
    int running = 1;

    while (running) {
        updateCamera();
        if (!control()) running = 0;
        renderScene(renderer);
        SDL_Delay(16); // Aproximadamente 60 FPS
    }
}

int main(int argc, char **argv) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!initSDL(&window, &renderer)) {
        return -1;
    }

    gameLoop(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
