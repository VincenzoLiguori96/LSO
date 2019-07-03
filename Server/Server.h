#include "../costanti.h"
typedef short int GameGrid[MAX_GRID_SIZE_L][MAX_GRID_SIZE_H];

int registerUser(char* newuser, char* newpassw);//Prende un nome utente, una password e file descriptor del file degli utenti e se non presente lo aggiunge

int checkUsername(char* username); // Ritorna la posizione delll'offset dello username all'interno del file se presente, -1 altrimenti

int logInUser(char* user, char* passw);//Effettua il login del utente passato

int createNewGame(GameGrid grid);//Crea un nuovo processo che inzializza una partita

int addPlayer(char *user, char** CurrentPlayer);//aggiunge un giocatore alla partita e aggiorna l'array di stringhe dei giocatori correnti

void endgame();

void readPlayerMovement();

int countdown();
