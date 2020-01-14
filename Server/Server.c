#include "Server.h"
#include <time.h>

int fdLog;

Game ** sessionGames;

int main(int argc, char* argv[]){

    clear();
    //seed per la generazione di numeri casuali;
    srand(time(NULL));

    int i = 0;

    //signal handler set;
    signal(SIGINT, handleSignal);
    signal(SIGHUP, handleSignal);
    signal(SIGQUIT, handleSignal);
    signal(SIGTERM, handleSignal);
    signal(SIGKILL, handleSignal);

    int *thread_sd, sock, sockfd;
    pthread_t tid;

    sessionGames= mmap(NULL, (sizeof(Game**)*20), PROT_READ|PROT_WRITE,
                        MAP_SHARED|MAP_ANONYMOUS, -1, 0);


    struct sockaddr_in client_addr;
    socklen_t client_len;

    LogServerStart(&fdLog);

    if((sock = creaSocket())<0){
      if(sock == ERR_SOCKET_CREATION){
        printf("%s", SOCKET_CREATION_ERR_MESSAGE);
        LogErrorMessage(&fdLog, SOCKET_CREATION_ERR_MESSAGE);
      }else if(sock = ERR_SOCKET_BINDING){
        printf("%s", SOCKET_BINDING_ERR_MESSAGE);
        LogErrorMessage(&fdLog, SOCKET_BINDING_ERR_MESSAGE);
      }else{
        printf("%s", NOT_SURE_ERR_MESSAGE);
        LogErrorMessage(&fdLog, NOT_SURE_ERR_MESSAGE);
      }
    }else{
      if((listen(sock,MAXIMUM_SOCKET_BACKLOG))<0){
        printf("%s", SOCKET_LISTEN_ERR_MESSAGE);
        LogErrorMessage(&fdLog,SOCKET_LISTEN_ERR_MESSAGE);
      }else{
        while(1){
          client_len = sizeof(client_addr);
          if((sockfd = accept(sock, NULL, NULL))<0){
            printf("%s", ACCEPT_SOCKET_ERR_MESSAGE);
            LogErrorMessage(&fdLog, ACCEPT_SOCKET_ERR_MESSAGE);
            exit(-1);
          }

          thread_sd = (int *) malloc(sizeof(int));
          thread_sd = &sockfd;

          if((pthread_create(&tid, NULL, gestisci, (void *) thread_sd))<0){
              printf("%s", THREAD_CREATION_ERR_MESSAGE);
              LogErrorMessage(&fdLog, THREAD_CREATION_ERR_MESSAGE);
          }
        }
      }
    }
    close(sock);
    close(sockfd);
    LogServerClose(&fdLog);
    return 1;
}


/*
  La funzione initializeNewGameProcess si occupa di creare la fork per una nuova partita.
*/
void initializeNewGameProcess(int sockfd, char user[]){

  int gameId;
  if((gameId=fork())<0){
    /*Gestire errore nuova partita*/
  }else if(gameId==0){
    char matrix[2000];
    char msg[50];
    int n_b_r;
    Game *g=NULL;
    g=createGame();
    strcpy(g->giocatori[0].nome,user);
    g->giocatori[0].nome[strlen(user)]='\0';
  
    if(createGameGrid(g) == 0){
      pthread_mutex_lock(&g->sem);
      g->gameId = getpid();

      LogPlayerJoin(&fdLog, g->gameId, user);
      GameGridToText(g->grid,matrix,1);
      pthread_mutex_unlock(&g->sem);
      sprintf(msg,"%ld",strlen(matrix));
      write(sockfd,msg,strlen(msg));
      n_b_r=read(sockfd,msg,5);
      write(sockfd,matrix,strlen(matrix));
      playGame(g,0,g->gameId);
    }else{
      sprintf(msg,"%ld",strlen(NO_CONNECTION_ERR_MESSAGE));
      write(sockfd,msg,strlen(msg));
      n_b_r=read(sockfd,msg,5);
      write(sockfd,NO_CONNECTION_ERR_MESSAGE,strlen(NO_CONNECTION_ERR_MESSAGE));

    }
  }else{
    LogNewGame(&fdLog,gameId);
  }
  return;
}


/*
  La funzione che esegue ogni nuovo thread;
*/
void * gestisci(void *arg){
  Game* current;
  char msg[1000];
  int n_b_r;
  int sockfd=*((int *) arg);
  sprintf(msg,"%ld",strlen(WELCOME_MESSAGE));
  write(sockfd,msg,strlen(msg));
  n_b_r=read(sockfd,msg,5);
  write(sockfd,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));
  n_b_r=read(sockfd,msg,50);
  msg[n_b_r]='\0';
  char user[50];
  int baba=-1;
  while(baba==-1){
    if(n_b_r==1){
      switch (msg[0]) {
        case 'l': case 'L':
          logInUserMenu(sockfd,user,&fdLog);
          baba=1;
          break;
        case 'r': case 'R':
          signInUserMenu(sockfd,user,&fdLog);
          baba=1;
          break;
        case 'e': case 'E':
          clear();
          write(sockfd,"-1", strlen("-1"));
          n_b_r = read(sockfd, msg, 50);
          if(strcmp(msg, USER_LOG_OUT)){
            LogUnkownClientDisconnection(&fdLog);
          }
          break;
        default:
          clear();
          sprintf(msg,"%ld",strlen(WELCOME_MESSAGE));
          write(sockfd,msg,strlen(msg));
          n_b_r=read(sockfd,msg,5);
          write(sockfd,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));
          n_b_r=read(sockfd,msg,50);
          msg[n_b_r]='\0';
          break;
      }
    }else{
          clear();
          sprintf(msg,"%ld",strlen(WELCOME_MESSAGE));
          write(sockfd,msg,strlen(msg));
          n_b_r=read(sockfd,msg,5);
          write(sockfd,WELCOME_MESSAGE,strlen(WELCOME_MESSAGE));
          n_b_r=read(sockfd,msg,50);
          msg[n_b_r]='\0';
    }
  }
  read(sockfd,msg,1);
  sprintf(msg,"%ld",strlen(GAME_SELECTION_MENU));
  write(sockfd,msg,strlen(msg));
  n_b_r=read(sockfd,msg,5);
  write(sockfd,GAME_SELECTION_MENU,strlen(GAME_SELECTION_MENU));
  n_b_r=read(sockfd,msg,5);
  msg[n_b_r]='\0';
  baba=-1;
  while(baba==-1){
    switch (msg[0]) {
      case 'n': case 'N':
        initializeNewGameProcess(sockfd, user);
        baba=1;
        break;
      case 'j': case 'J':
      //  current = joinGame(sockfd,user, fdLog, &sessionGames);
        if(current == NULL){
          sprintf(msg, "%ld", strlen(NO_SUCH_GAMES_MESSAGE));
          write(sockfd,msg, strlen(msg));
          n_b_r=read(sockfd, msg, 5);
          write(sockfd, NO_SUCH_GAMES_MESSAGE, strlen(NO_SUCH_GAMES_MESSAGE));
          n_b_r=read(sockfd,msg,5);
        }else{
          char matrix[2000];
          pthread_mutex_lock(&current->sem);
          LogPlayerJoin(&fdLog, current->gameId, user);
          GameGridToText(current->grid,matrix,1);
          sprintf(msg,"%ld",strlen(matrix));
          write(sockfd,msg,strlen(msg));
          n_b_r=read(sockfd,msg,5);
          write(sockfd,matrix,strlen(matrix));
          pthread_mutex_unlock(&current->sem);
          baba=1;
        }
        break;
      case 'e': case 'E':
        clear();
        write(sockfd, "-1", strlen("-1"));
        n_b_r = read(sockfd, msg, 50);
        if(strcmp(msg, USER_LOG_OUT)){
          LogUserSignOut(&fdLog, user);
        }
        break;
      default:
        sprintf(msg,"%ld",strlen(GAME_SELECTION_MENU));
        write(sockfd,msg,strlen(msg));
        n_b_r=read(sockfd,msg,5);
        write(sockfd,GAME_SELECTION_MENU,strlen(GAME_SELECTION_MENU));
        n_b_r=read(sockfd,msg,50);
        msg[n_b_r]='\0';
        break;
    }
  }
}


void handleSignal(int Sig){
  if(Sig == SIGINT){
    LogServerClose(&fdLog);
    //unlink(SOCKET);
    exit(1);
  }
}

void deleteGrid(GameGrid **g){
  int i;
  for(i=0;i<MAX_GRID_SIZE_H;i++)
    free(g[i]);
  free(g);
  return;
}

Game* joinGame(int sockfd, char user[], int fdLog){
  printf("prima di printList.\n");

  return NULL;
}
