#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

struct Coordinates
{
    float x;
    float y;
};
struct Game
{
    int status; //(0- WAITING_PLAYERS, 1- STARTED, -1- ENDED);
    char name[20];//* Uzvarētāja spēlētāja ID, vai -1, ja tāda vēl nav * 
    int WinnerPlayerID;
};
struct Player_info
{
    int ID; /*Spēlētāja ID*/
    char name[30];/* Spēlētāja vārds */
    struct Coordinates position; /* Spēlētāja koordinātas */
    float angle; /* Spēlētāja leņķis */
    float speed; /*Spēlētāja ātrums*/
    float acceleration; /*Spēlētāja paātrinājums*/
    int laps; /* Cik reizes spēlētājs jau apbraucis apkārt kartei */
};
struct Field
{
    int ID; //* Laukuma ID *
    char name[20]; //* Laukuma nosaukums *
    int Width; //* Skaitlis, kas raksturo laukuma platumu  *
    int Height; //* Skaitlis, kas raksturo laukuma augstumu  *
};
struct Line{
    struct Coordinates beggining;
    struct Coordinates end;
};
struct Action{ // domāts kā arrow key inputs
    int x; //(1, pa labi -1 pa kreisi, 0 bez virziena)
    int y; //( 1 uz priekšu, -1 atpakaļ 0 bez virziena)
};

#endif