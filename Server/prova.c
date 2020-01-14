#include "Server.h"
#include <malloc.h>

void setPermessi(int x, int y, int giocatore, GameGrid ** grid){
  switch (giocatore) {
    case 0:
      grid[y][x].p0 = 1;
      break;
    case 1:
      grid[y][x].p1 = 1;
      break;
    case 2:
      grid[y][x].p2 = 1;
      break;
    case 3:
      grid[y][x].p3 = 1;
      break;
    case 4:
      grid[y][x].p4 = 1;
      break;
    case 5:
      grid[y][x].p5 = 1;
      break;
    case 6:
      grid[y][x].p6 = 1;
      break;
    case 7:
      grid[y][x].p7 = 1;
      break;
    default:
      printf("this should not be reachable.\n");
      break;
  }
  return;
}

Game *createGame(){
  Game* temp;
  temp=(Game*)malloc(sizeof(Game));
  temp->grid=NULL;
  pthread_mutex_init(&(temp->sem),NULL);
  return temp;
}

int azioneGiocatore(Game *game, int giocatore, char action, int gameId){

  pthread_mutex_lock(&game->sem);

  player *player = &game->giocatori[giocatore];
  GameGrid ** grid = game->grid;
  int x=player->posx,y=player->posy;
  int destx;
  int desty;
  switch (action) {
    case 'w': case 'W':
    if(y>0){
      setPermessi(x,y-1,giocatore,grid);
      if(grid[y-1][x].ostacolo || grid[y-1][x].giocatore){
        printf("ostacolo o giocatore\n" );
        /*Gestire mossa non valida*/
      }else{
        grid[y][x].giocatore=0;
        grid[y-1][x].giocatore=1;
        grid[y-1][x].codiceGiocatore=giocatore;
        player->posy=y-1;
      }
    }else{
      printf("stai fuori\n" );
      /*Gesire mossa non valida*/
    }
    break;
    case 'a': case 'A':
      if(x>0){
        setPermessi(x-1,y,giocatore,grid);
        if(grid[y][x-1].ostacolo || grid[y][x-1].giocatore){
          printf("ostacolo o giocatore\n" );
          /*Gestire mossa non valida*/
        }else{
          grid[y][x].giocatore=0;
          grid[y][x-1].giocatore=1;
          grid[y][x-1].codiceGiocatore=giocatore;
          player->posx=x-1;
        }
      }else{
        printf("stai fuori\n" );
        /*Gesire mossa non valida*/
      }
    break;
    case 's': case 'S':
    if(y<MAX_GRID_SIZE_H){
      setPermessi(x,y+1,giocatore,grid);
      if(grid[y+1][x].ostacolo || grid[y+1][x].giocatore){
        printf("ostacolo o giocatore\n" );
        /*Gestire mossa non valida*/
      }else{
        grid[y][x].giocatore=0;
        grid[y+1][x].giocatore=1;
        grid[y+1][x].codiceGiocatore=giocatore;
        player->posy=y+1;
      }
    }else{
      printf("stai fuori\n" );
      /*Gesire mossa non valida*/
    }
    break;
    case 'd': case 'D':
      if(x<MAX_GRID_SIZE_L){
        setPermessi(x+1,y,giocatore,grid);
        if(grid[y][x+1].ostacolo || grid[y][x+1].giocatore){
          printf("ostacolo o giocatore\n" );
          /*Gestire mossa non valida*/
        }else{
          grid[y][x].giocatore=0;
          grid[y][x+1].giocatore=1;
          grid[y][x+1].codiceGiocatore=giocatore;
          player->posx=x+1;
        }
      }else{
        printf("stai fuori\n" );
        /*Gesire mossa non valida*/
      }
    break;
    case 'q': case 'Q':
    break;
    case 'e': case 'E':
    break;
    case 'r': case 'R':
    break;
    case 'l': case 'L':
    break;
    case '0':
    break;
    default:
    break;
  }
  pthread_mutex_unlock(&game->sem);
}

int main(int argc, char const *argv[]) {

  srand(time(NULL));
  Game *g=createGame();

  clear();
  char msg[4000];
  unsigned int giocatore=1;
  int i, j, x, y, numOstacoli=0;
  GameGrid **p=(GameGrid**)malloc(MAX_GRID_SIZE_H * sizeof(GameGrid*));

  if(p!=NULL){
    for(i=0;i<MAX_GRID_SIZE_H;i++)
      p[i]=(GameGrid*)calloc(MAX_GRID_SIZE_L, sizeof(GameGrid));
    y=rand()%MAX_GRID_SIZE_H;
    x=rand()%MAX_GRID_SIZE_L;
    p[y][x].giocatore=1;
    p[y][x].p0=1;
    p[y][x].codiceGiocatore=0;
    g->giocatori[0].posx=x;
    g->giocatori[0].posy=y;
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_H;
        x=rand()%MAX_GRID_SIZE_L;
      } while(p[y][x].giocatore);
      p[y][y].pacco=1;
      p[y][y].p0=1;
      p[y][y].p1=1;
      p[y][y].p2=1;
      p[y][y].p3=1;
      p[y][y].p4=1;
      p[y][y].p5=1;
      p[y][y].p6=1;
      p[y][y].p7=1;
      p[y][y].codicePacco=i;
    }
    for (i=0; i < MAX_PACCHI; i++) {
      do {
        y=rand()%MAX_GRID_SIZE_H;
        x=rand()%MAX_GRID_SIZE_L;
      } while(p[y][x].giocatore || p[y][x].pacco);
      p[y][x].locazione=1;
      p[y][x].p0=1;
      p[y][x].p1=1;
      p[y][x].p2=1;
      p[y][x].p3=1;
      p[y][x].p4=1;
      p[y][x].p5=1;
      p[y][x].p6=1;
      p[y][x].p7=1;
      p[y][x].codiceLocazione=i;
    }
    for(i=0;i<MAX_OBSTACLES_N;i++){
      y=rand()%MAX_GRID_SIZE_H;
      x=rand()%MAX_GRID_SIZE_L;
      if(!(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione)){
        p[y][x].ostacolo=1;
        numOstacoli++;
      }
    }
    while(numOstacoli<MIN_OBSTACLES){
      y=rand()%MAX_GRID_SIZE_H;
      x=rand()%MAX_GRID_SIZE_L;
      if(!(p[y][x].giocatore || p[y][x].pacco || p[y][x].locazione)){
        p[y][x].ostacolo=1;
        numOstacoli++;
      }
    }
  }else{
    return -1;
  }
  g->grid=p;
  char mom[15];
  int k=1;
  do{
    for(i=0;i<MAX_GRID_SIZE_L+1;i++){
    if(i!=0){
      if(i/10==0)
      sprintf(mom,"%d   ",i);
      else
        sprintf(mom,"%d  ",i);
      strcat(msg,mom);
    }else{
      sprintf(mom,"    ");
      strcat(msg, mom);
    }
    }
    sprintf(mom, "\n\n\n");
    strcat(msg, mom);
    for(i=0;i<MAX_GRID_SIZE_H;i++){
      if(i+1!=10)
        sprintf(mom,"%d   ",i+1);
      else
        sprintf(mom,"%d  ",i+1);
      strcat(msg,mom);
      for(j=0;j<MAX_GRID_SIZE_L;j++){
        if(!p[i][j].p0)
          strcat(msg,"?   ");
        else{
          if (p[i][j].ostacolo) {
            strcat(msg,"\033[91m\033[91mX   \033[0m");
          }
          else{
            if (p[i][j].giocatore) {
              if (p[i][j].codiceGiocatore==0) {
                strcat(msg,"\033[92m\033[92mME  \033[0m");
              }else{
                strcat(msg,"\033[92m\x1b[35mEN  \033[0m");
              }
            }else{
              if (p[i][j].pacco) {
                strcat(msg,"\033[93m\033[93mP   \033[0m");
              } else {
                if(p[i][j].locazione){
                  strcat(msg,"\033[96m\033[96mL   \033[0m");
                }else{
                  strcat(msg,"[]  ");
                }
              }
            }
          }
        }
      }
      strcat(msg,"\n\n");
    }
    char w='D';
    printf("posizione giocatore x=%d,y=%d\n",g->giocatori[0].posx+1,g->giocatori[0].posy+1);
    printf("%s\n\n",msg);
    msg[0]='\0';
    azioneGiocatore(g,0,w,13);
  }while(k++<4);

  return 0;
}