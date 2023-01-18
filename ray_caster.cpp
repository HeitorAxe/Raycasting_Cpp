#include "ray_caster.h"

Player::Player(double posX, double posY,double FOVDegrees)
{
    this->planeY = FOVDegrees/100;
    this->posX = posX;
    this->posY = posY;
}

void Player::moveForward(double moveSpeed)
{
    this->posX += this->dirX * moveSpeed;
    this->posY += this->dirY * moveSpeed;
}

void Player::rotateClockwise(double rotSpeed)
{
    double oldDirX = this->dirX;
    this->dirX = this->dirX * cos(rotSpeed) - this->dirY * sin(rotSpeed);
    this->dirY = oldDirX * sin(rotSpeed) + this->dirY * cos(rotSpeed);
    double oldPlaneX = this->planeX;
    this->planeX = this->planeX * cos(rotSpeed) - this->planeY * sin(rotSpeed);
    this->planeY = oldPlaneX * sin(rotSpeed) + this->planeY * cos(rotSpeed);
}


void rayCaster(SDL_Renderer* renderer, int screenWidth, int screenHeight, int worldMap[24][24], Player player, double****texture)
{

    int w = screenWidth;
    for(int x = 0; x < w; x++)
    {
        //calculate ray position and direction

        //X coordinate on camera space
        double cameraX = 2*x/double(w)-1;
        //The ray will start at the player position and go in rayDir x and y direction unitill find a wall
        double rayDirX = player.dirX + player.planeX *cameraX;
        double rayDirY = player.dirY + player.planeY *cameraX;

        //Wich box of the map we are in
        //rounds our position to a tila in the map
        int mapX = int(player.posX);
        int mapY = int(player.posY);

        //length of ray from current position to next x or y side
        double sideDistX;
        double sideDistY;

        //Length of one x or y side to the next
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);
        //Will be used to calculate length of the ray
        double perpWallDist;

        //What direction to step in x or y ( will aways be 1 or -1)
        int stepX;
        int stepY;

        //where the wall was hit
        int hit = 0;
        // 0 if x side wall was hit and 1 if y side was hit
        int side;


        //Calculating stepX, stepY and initial sideDistX and sideDistY
        //rayDirX < 0 == ray goes left direction
        //rayDirY < 0 == ray goes downward direction
        // if positive, it is the inverse 
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (player.posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (player.posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
        }
        

        //DDA
        //we are now checking every square in the rays direction
        //untill we find a wall
        //each step, the actual distance is updated using deltaDist's
        while(hit == 0)
        {
            //go to the next map square, either in x direction or in y direction
            //if sideDistX is closest
            if(sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            //check if wall was hit
            if(worldMap[mapX][mapY] > 0) hit = 1; //printf("HIT\n");
        }

        //Now we gotta calculate the distance projected on the
        //camera direction, so we can avoid the fisheye effect
        //which would happen if we were to use actual euclidean
        //distance

        if(side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        //now we calculate the height of the column drawn to the screen
        int columnHeight = (int)(screenHeight/perpWallDist);

        //lowest and heighest pixel of the column
        //the conditions are to cap the size to be the screen size at max
        int drawStart = -columnHeight/2 + screenHeight/2;
        if(drawStart < 0)
            drawStart = 0;
        int drawEnd = columnHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight)
            drawEnd = screenHeight - 1;

        //drawing the column
/*         SDL_Rect rect;
        rect.h = columnHeight;
        rect.w = screenWidth/w;
        rect.x = (screenWidth/w) * x;
        rect.y = drawStart; */

        //Adjusting Textures!!!
        int texNum = worldMap[mapX][mapY] - 1;//wich texture wil be selected
        //exact coordinate on where the ray hit the wall
        double wallX;
        if (side == 0) wallX = player.posY + perpWallDist * rayDirY;
        else           wallX = player.posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        //should make a texture object with this properties
        int texWidth =32;
        int texHeight =32;

        //x coordinate of the texture we will draw
        int texX = int(wallX * double(texWidth));
        if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

        //calculating y coordinate of the texture
        double step = 1.0 * texHeight/columnHeight;
        //starting texture coordinate
        double texPos = (drawStart - screenHeight / 2 + columnHeight / 2) * step;
        
        //this works but is really slow
        //especially when to close to wall wich means more
        //pixels will have to be drawn
        for(int y = drawStart; y<drawEnd; y++)
        {
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;
            //printf("%d\n", texX);
            int r = texture[texNum][texY][texX][0];
            int g = texture[texNum][texY][texX][1];
            int b = texture[texNum][texY][texX][2];
            if(side == 1)SDL_SetRenderDrawColor(renderer, r/2, g/2, b/2, 255);
            else SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, x, y);

        }

    }
}

