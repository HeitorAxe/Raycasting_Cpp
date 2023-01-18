
// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include "ray_caster.h"
#include "pics/default.ppm"
#include "pics/guardiaWall.ppm"
#include "pics/guardiaWallSkull.ppm"
#include "pics/guardiaWallAngel.ppm"
#include "pics/timeFence.ppm"
#include "pics/postlight.ppm"




// Command for compiling it g++ *.cpp -w -lSDL2 -o main
//i did this following this tutorial: https://lodev.org/cgtutor/raycasting.html
#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480
#define moveSpeed .1
#define rotSpeed .1
#define fps 30

// Global Variables
// The window we'll be rendering to
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

int worldMap[mapWidth][mapHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

// Starts up SDL and creates window
bool init()
{
    // Initialization Flag
    bool success = true;

    // Initizalizing SDL

    // if it was not succesfully initialized
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Create Window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        // Creates Renderer
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
    }

    return success;
}

void loadTextureIn(double ***texture, int*textureFile,int res)
{
     for(int y= 0; y<res; y++)
    {
        for(int x = 0; x<res; x++)
        {
            int pixel =(y*res+x)*3;
            texture[y][x][0] = textureFile[pixel];
            texture[y][x][1] = textureFile[pixel+1];
            texture[y][x][2] = textureFile[pixel+2];
        }
    }
}

double **** textureLoad(int texWidth, int texHeight)
{
    double**** texture=0;
    texture = new double***[5];

    for(int h = 0; h < 8; h++)
    {
        texture[h] = new double**[texHeight];
        for(int w = 0; w<texHeight;w++)
        {
            texture[h][w] = new double*[texWidth];
            for(int i = 0; i<texWidth; i++)
            {
                texture[h][w][i] = new double[3];
            }
        }
    }
    


    //generate default textures
    for(int x = 0; x < texWidth; x++)
    for(int y = 0; y < texHeight; y++)
    {
        for(int i = 0; i < 3; i++){
            if(x == texWidth/2 && y == texHeight/2)
            {
                texture[0][y][x][i] = 0;
                texture[1][y][x][i] = 0;
                texture[2][y][x][i] = 0;
                texture[3][y][x][i] = 0;
                texture[4][y][x][i] = 0;
                texture[5][y][x][i] = 0;
            }else{
                texture[0][y][x][i] = 255;
                texture[1][y][x][i] = 255;
                texture[2][y][x][i] = 255;
                texture[3][y][x][i] = 255;
                texture[4][y][x][i] = 255;
                texture[5][y][x][i] = 255;
            }
        }

    }

    loadTextureIn(texture[0], guardiaWall, texWidth);
    loadTextureIn(texture[1], guardiaWallSkull, texWidth);
    loadTextureIn(texture[2], guardiaWallAngel, texWidth);
    //loadTextureIn(texture[2], postlight, texWidth);



    return texture;
}

int main(int argc, char *args[])
{
    //player starting position and degrees of field of view;
    Player player(22, 22, 65);

    double****texture = textureLoad(32, 32);


    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // main loop flag
        bool quit = false;

        SDL_Event e;

        int desiredDelta = 1000 / fps;

        //main game loop
        while (!quit)
        {

            int startLoop = SDL_GetTicks();
            // Handle events on queue
            // keeps processing event queue untill it is empty
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                // User presses a key
                else if (e.type == SDL_KEYDOWN)
                {
                    // Select surfaces based on key press
                    switch (e.key.keysym.sym)
                    {

                    case SDLK_UP:
                        // SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                        //the moveForward function doesnt check for wall in the world map
                        //therefore we should use the following conditions to check it
                        if (worldMap[int(player.posX + player.dirX * moveSpeed)][int(player.posY)] == false
                        && worldMap[int(player.posX)][int(player.posY + player.dirY * moveSpeed)] == false)
                            player.moveForward(moveSpeed);
                        //printf("player.POSX: %f/ POSY: %f\n", player.posX, player.posY);
                        //printf("DIRX: %f/ DIRY: %f\n", player.dirX, player.dirY);
                        break;

                    case SDLK_DOWN:
                        if (worldMap[int(player.posX - player.dirX * moveSpeed)][int(player.posY)] == false &&
                        worldMap[int(player.posX)][int(player.posY - player.dirY * moveSpeed)] == false)
                            player.moveForward(-moveSpeed);

                        break;

                    case SDLK_LEFT:
                        player.rotateClockwise(rotSpeed);
                        break;

                    case SDLK_RIGHT:
                        player.rotateClockwise(-rotSpeed);
                        break;

                    default:

                        break;
                    }
                }

                SDL_SetRenderDrawColor(gRenderer, 37, 6, 0, 255);
                SDL_RenderClear(gRenderer);
                SDL_SetRenderDrawColor(gRenderer,0, 0, 0, 255);
                SDL_Rect rect;
                rect.x = 0;
                rect.y = 0;
                rect.w = screenWidth;
                rect.h = screenHeight/2;
                SDL_RenderFillRect(gRenderer, &rect);
                rayCaster(gRenderer, screenWidth, screenHeight, worldMap, player, texture);
                SDL_RenderPresent(gRenderer);

                int delta = SDL_GetTicks() - startLoop;
                if (delta < desiredDelta)
                {
                    SDL_Delay(desiredDelta - delta);
                }
            }
        }
    }
}