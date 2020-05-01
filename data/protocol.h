
#include "./constant.h"
#include "./threadparam.h"
#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

struct Field{
    int ID; //* Laukuma ID *
    char name[20]; //* Laukuma nosaukums *
    int Width; //* Skaitlis, kas raksturo laukuma platumu  *
    int Height; //* Skaitlis, kas raksturo laukuma augstumu  *
};

typedef struct Action{ // domāts kā arrow key inputs
    int x; //(1, pa labi -1 pa kreisi, 0 bez virziena)
    int y; //( 1 uz priekšu, -1 atpakaļ 0 bez virziena)
}action_t;

struct Line{
    struct Coordinates beggining;
    struct Coordinates end;
};

#endif