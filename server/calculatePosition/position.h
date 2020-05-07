#include <math.h>

#ifndef CALCULATEPOS_H_INCLUDED
#define CALCULATEPOS_H_INCLUDED

void updatePosition(player_t** player, action_t action){
    player_t* currentPlayer = *player;
    if(action.x == 1){ //Turn right
        currentPlayer->angle -= 0.25;

    }
    else if(action.x == -1){//Turn 
        currentPlayer->angle += 0.25;

    }
    if(action.y == 1){//move forward
        return;
    }
    else if(action.y == -1){//move backward{
        currentPlayer->y -= 1;
        return;
    }
    return;
}

#endif // !CALCULATEPOS_H_INCLUDED
