#include <math.h>

#ifndef CALCULATEPOS_H_INCLUDED
#define CALCULATEPOS_H_INCLUDED

#define ACCELERATION     0.08
#define MAX_SPEED        0.95
#define MAX_REV_SPEED    -0.6
#define TURN_SPEED       0.05
#define FRICTION         0.03
#define MAP_HEIGHT       40
#define MAP_WIDTH        150

bool checkMove (double y1, double x1, double y2, double x2){
    int intx1 = round(x1);
    int inty1 = round(y1);
    int intx2 = round(x2);
    int inty2 = round(y2);
    int realx, realy;

        //creates the loop of the map
    for(int y = 2; y < MAP_HEIGHT-1;y++){
        for(int x = 1; x < MAP_WIDTH-1; x++){
            realx = x - MAP_WIDTH/2;
            realy = (y - MAP_HEIGHT/2)*3;

            if((realx)*(realx) + (realy)*(realy) > (MAP_WIDTH/2) * (MAP_WIDTH/2)){
                if ((intx1 == realx && inty1 == realy) || (intx2 == realx && inty2 == realy)) return false;
            }
        }
    }
        //creates the middle of the loop
    for(int y = 10; y < MAP_HEIGHT-10;y++){
        for(int x = 5; x < MAP_WIDTH- 8; x++){
            realx = x - MAP_WIDTH/2;
            realy = (y - MAP_HEIGHT/2)*3;

            if((realx)*(realx) + (realy)*(realy) < ((MAP_HEIGHT)/2) * (MAP_HEIGHT/2) * 6){
                if ((intx1 == realx && inty1 == realy) || (intx2 == realx && inty2 == realy)) return false;
            }
        }
    }

    return true;
}

void updatePosition(player_t** player, action_t action){
    player_t* currentPlayer = *player;

    if ((*player)->speed > 0.01){
        (*player)->speed -= FRICTION;
    } else if ((*player)->speed < 0.01){
        (*player)->speed += FRICTION;
    }

    float tempVelocity = (*player) -> speed;
    float tempAngle = (*player) -> angle;

        //if w or s was input, add acceleration in this direction
     tempVelocity += ACCELERATION*action.y;
     tempAngle -= TURN_SPEED*action.x;

        if (tempVelocity > MAX_SPEED)
        tempVelocity = MAX_SPEED;
    else if (tempVelocity < MAX_REV_SPEED)
        tempVelocity = MAX_REV_SPEED;

        
        //is basically the x and y components 
            //added + speed in the angle direction
            ///should be cos/sin^2 but want to save resouces.
    float newXHead = 
          ((*player)->x)       //midmark
        + cos(tempAngle * M_PI)                 //comepnsation of the angle for the body
        + tempVelocity * cos(tempAngle * M_PI); //to be added in order to move forwards

    float newYHead = 
           ((*player)->y) 
        + sin(tempAngle * M_PI)
        + tempVelocity * sin(tempAngle * M_PI);

    float newXTail = ((*player)->x) 
        - cos(tempAngle * M_PI)
        + tempVelocity * cos(tempAngle * M_PI);

    float newYTail = ((*player)->y) 
        - sin(tempAngle * M_PI)
        + tempVelocity * sin(tempAngle * M_PI);
        
        (*player)->x += cos(tempAngle * M_PI)*action.y;
        (*player)->y += sin(tempAngle * M_PI)*action.y;
        (*player)->angle -= (TURN_SPEED * M_PI)*action.x;

        //if the new coordinates of the head and tail don't collide,
            //parameters get updated
    if(checkMove(newYHead, newXHead, newYTail, newXTail)){
        (*player)->x += cos(tempAngle * M_PI)*action.y;
        (*player)->y += sin(tempAngle * M_PI)*action.y;
        (*player)->angle -= (TURN_SPEED * M_PI)*action.x;
        //(*player)->speed = tempVelocity;
    } else { //otherwise, collision detected and speed is zero. 
        (*player)-> speed = 0;
    }

    return;
}

//checks if player has been in all 4 quadrants of the map. 
    //with powers of 2, it can be determined which one is missing
    //(essentially midmarks are 0 - 15 or 0000 - 1111 in binary)
void checkLapCrosing(player_t** player){

            // 4 > < | 8 > > 
            //---------------
            // 1 < < | 2 < >

    if      ((*player)->y < MAP_HEIGHT/2 && (*player)->x < MAP_WIDTH/2)
    {
        if ((*player)->midMark % 2 < 1) (*player)->midMark += 1;
    } 
    else if ((*player)->y < MAP_HEIGHT/2 && (*player)->x > MAP_WIDTH/2)
    {
        if ((*player)->midMark % 4 < 2) (*player)->midMark += 2;
    } 
    else if ((*player)->y > MAP_HEIGHT/2 && (*player)->x < MAP_WIDTH/2)
    {
        if ((*player)->midMark % 8 < 4) (*player)->midMark += 4;
    } 
    else 
    {
        if ((*player)->midMark % 16 < 8) (*player)->midMark += 8;
    }

        //if all midMarks collected, looks for finish line
    if ((*player)->midMark == 15){

        int intx = round((*player)->x);
        int inty = round((*player)->y);
        if (intx == 74 && inty > MAP_HEIGHT){
            (*player)->laps++;
            (*player)->midMark = 0; 
            return;
        }
    }
}

#endif // !CALCULATEPOS_H_INCLUDED
