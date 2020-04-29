#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

struct Field
{
    int ID; //* Laukuma ID *
    char name[20]; //* Laukuma nosaukums *
    int Width; //* Skaitlis, kas raksturo laukuma platumu  *
    int Height; //* Skaitlis, kas raksturo laukuma augstumu  *
};

typedef struct Action{ // domāts kā arrow key inputs
    int x; //(1, pa labi -1 pa kreisi, 0 bez virziena)
    int y; //( 1 uz priekšu, -1 atpakaļ 0 bez virziena)
}action_t;

typedef struct Coordinates
{
    float x;
    float y;
}coordinate_t;

struct Line{
    struct Coordinates beggining;
    struct Coordinates end;
};

typedef struct Player_info
{
    int ID; /*Spēlētāja ID*/
    int gameID;
    char name[30];/* Spēlētāja vārds */
    coordinate_t position; /* Spēlētāja koordinātas */
    float angle; /* Spēlētāja leņķis */
    float speed; /*Spēlētāja ātrums*/
    float acceleration; /*Spēlētāja paātrinājums*/
    int laps; /* Cik reizes spēlētājs jau apbraucis apkārt kartei */
    struct Player_info* next;
    struct Player_info* prev;
}player_t;


typedef struct Player_list{
    int count;
    player_t* head;
    player_t* tail;
}playerlist_t;

typedef struct Game
{
    int gameid;
    int status; //(0- WAITING_PLAYERS, 1- STARTED, -1- ENDED);
    int hostId;
    playerlist_t* playerlist;
    struct Game* next;
    struct Game* prev;
}game_t;

typedef struct Game_list{
    int count;
    game_t* head;
    game_t* tail;
}gamelist_t;

typedef struct threadParam{
    int clientFd;
    int serverFd;
    playerlist_t* clientlist;
    gamelist_t* list;
}params_t;

#endif