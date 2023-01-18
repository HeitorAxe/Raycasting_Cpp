#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include<SDL2/SDL.h>

class Player{
    public:
        //Players Position
        double posX;
        double posY;

        //Initial vector direction
        double dirX = -1;
        double dirY = 0;

        //Camera "plane" (its actually 2d), determines FOV
        double planeX = 0;
        double planeY;
        Player(double posX, double posY, double FOVDegrees);
        void rotateClockwise(double rotSpeed);
        void moveForward(double moveSpeed);

};


void rayCaster(SDL_Renderer* renderer, int screenWidth, int screenHeight, int worldMap[24][24], Player player, double****texture);
//void floorRayCaster(SDL_Renderer* renderer, int screenWidth, int screenHeight, int worldMap[24][24], Player player, double****texture);

#endif