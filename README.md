# Multiplayer_racing
Multiplayer racing is a client server multiplayer game developed in C using nCurses for graphics.
Implements a terminal launched game including a lobby system and floating point physics.

Compile with with gcc:
Server:
gcc server/server.c -o s -lpthread && ./s

Client:
gcc client/client.c -o c -lncurses -lm -lpthread && ./c
