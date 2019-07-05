#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../costanti.h"
void printGameGrid(GameGrid grid);

int logInRequest(char* username, char * password);

int signInRequest(char* username, char* password);

int newGame();

int joinGame();

int movementRequest();
