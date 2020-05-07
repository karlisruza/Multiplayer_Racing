#include <math.h>

#ifndef CALCULATEPOS_H_INCLUDED
#define CALCULATEPOS_H_INCLUDED

#define ACCELERATION     0.08;
#define MAX_SPEED        0.95;
#define MAX_REV_SPEED    -0.6;
#define TURN_SPEED       0.097;
#define FRICTION         0.03;


#define MAP_HEIGHT = 40;
#define MAP_WIDTH  = 150;

void updatePosition(player_t** player, action_t action){
    player_t* currentPlayer = *player;

        //decrease of velocity naturally
    if (((*player) -> velocity) > 0.01){
        (*player) -> velocity -= FRICTION;
    } else if (((*player) -> velocity) < -0.01){
        (*player) -> velocity += FRICTION;
    } else 
        (*player) -> velocity = 0;


    float tempVelocity = (*player) -> velocity;
    float tempAngle = (*player) -> angle;

        //if w or s was input, add acceleration in this direction
    if (action.x != 0){
        if(tempVelocity >= MAX_SPEED || tempVelocity <= MAX_REV_SPEED){
            tempVelocity += ACCELERATION*action.x;
        }
    }
        //if a or d was pressed, add a turn in the said direction
    else 
        tempAngle += TURN_SPEED*action.y;

        
        //is basically the x and y components 
            //added + velocity in the angle direction
            ///should be cos/sin^2 but want to save resouces.
    double newXHead = ((*player)->mid->x)       //midmark
        + cos(tempAngle * M_PI)                 //comepnsation of the angle for the body
        + tempVelocity * cos(tempAngle * M_PI); //to be added in order to move forwards

    double newYHead = ((*player)->mid->y) 
        + sin(tempAngle * M_PI)
        + tempVelocity * sin(tempAngle * M_PI);

    double newXTail = ((*player)->mid->x) 
        - cos(tempAngle * M_PI)
        + tempVelocity * cos(tempAngle * M_PI);

    double newYTail = ((*player)->mid->y) 
        - sin(tempAngle * M_PI)
        + tempVelocity * sin(tempAngle * M_PI);

        //if the new coordinates of the head and tail don't collide,
            //parameters get updated
    if(checkMove(win, newYHead, newXHead) && checkMove(win, newYTail, newXTail)){
        (*player)->mid->x = newXHead - 1*cos((*player)->angle * M_PI);
        (*player)->mid->y = newYHead - 1*sin((*player)->angle * M_PI);
        (*player)->angle = tempAngle;
        (*player)->velocity = tempVelocity;
    } else { //otherwise, collision detected and velocity is zero. 
        (*player)->velocity = 0;
    }

    return;
}

bool checkMove (double y, double x){
    int intx = round(x);
    int inty = round(y);
    int realx, realy;


        //creates the loop of the map
    for(int y = 2; y < MAP_HEIGHT-1;y++){
        for(int x = 1; x < MAP_WIDTH-1; x++){
            realx = x - MAP_WIDTH/2;
            realy = (y - MAP_HEIGHT/2)*3;

            if((realx)*(realx) + (realy)*(realy) > (MAP_WIDTH/2) * (MAP_WIDTH/2)){
                if (realx == intx && realy == inty) return false;
            }
        }
    }
        //creates the middle of the loop
    for(int y = 10; y < MAP_HEIGHT-10;y++){
        for(int x = 5; x < MAP_WIDTH- 8; x++){
            realx = x - MAP_WIDTH/2;
            realy = (y - MAP_HEIGHT/2)*3;

            if((realx)*(realx) + (realy)*(realy) < ((MAP_HEIGHT)/2) * (MAP_HEIGHT/2) * 6){
                if (realx == intx && realy == inty) return false;
            }
        }
    }

    return true;
}

//checks if player has been in all 4 quadrants of the map. 
    //with powers of 2, it can be determined which one is missing
    //(essentially midmarks are 0 - 15 or 0000 - 1111 in binary)
void checkLapCrosing(player_t** player){

            // 4 > < | 8 > > 
            //---------------
            // 1 < < | 2 < >

    if      ((*player)->mid->y < MAP_HEIGHT/2 && (*player)->mid->x < MAP_WIDTH/2)
    {
        if ((*player)->midMark % 2 < 1) (*player)->midMark += 1;
    } 
    else if ((*player)->mid->y < MAP_HEIGHT/2 && (*player)->mid->x > MAP_WIDTH/2)
    {
        if ((*player)->midMark % 4 < 2) (*player)->midMark += 2;
    } 
    else if ((*player)->mid->y > MAP_HEIGHT/2 && (*player)->mid->x < MAP_WIDTH/2)
    {
        if ((*player)->midMark % 8 < 4) (*player)->midMark += 4;
    } 
    else 
    {
        if ((*player)->midMark % 16 < 8) (*player)->midMark += 8;
    }

        //if all midMarks collected, looks for finish line
    if ((*player)->midMark == 15){

        int intx = round((*player)->mid->x);
        int inty = round((*player)->mid->y);
        if (intx == 74 && inty > MAP_HEIGHT){
            (*player)->laps++;
            (*player)->midMark = 0; 
            return;
        }
    }
}

#endif // !CALCULATEPOS_H_INCLUDED
