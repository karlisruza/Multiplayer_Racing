#include <stdlib.h>
#include <stdio.h>
#include "protocol.h"

#ifndef PAYLOAD_H_INCLUDED
#define PAYLOAD_H_INCLUDED

//These structs define the data to be sent as the payload
typedef struct update_payload_type{
    int playerID;
    int gameID;
    action_t action;
}up_pt;


#endif