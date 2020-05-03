# LSP2020_racing_proj
Final Project for LU LSP course

Compile with with gcc:

Server:
gcc server/server.c -o s -lpthread && ./s

Client:
gcc client/client.c -o c -lncurses -lm -lpthread && ./c
