#include <stdio.h>
#include <stdlib.h>
#include "Client.h"

int main(){

  /*int inputInt;
  char[MAX_SIZE_USERNAME] inputUser;

  printf("Benvenuto in questo gioco di merda.\nCosa vuoi fare:\n1)Registrati.\n2)Accedi.\n3)Esci.\n");
  scanf("%d",&inputInt);
  while(inputInt<4&&inputInt>0)
  switch (inputInt) {
    case 1 :signInRequest();
    break;
    case 2: logInRequest();
    break;
    case 3: return 0;
    default: printf("Da 1 a 3 Bucchin\n");
    scanf("%d",&inputInt);
  }
  //Da questo punto in poi il Client è Loggato
  printf("Benvenuto %s.\nCosa vuoi fare:\n1)Nuova partita.\n2)Entra in una partita.\n3)Esci.", inputUser);
  scanf("%d",&inputInt);
  while(inputInt<4&&inputInt>0)
  switch (inputInt) {
    case 1 :newGame();
    break;
    case 2: joinGame();
    break;
    case 3: return 0;
    default: printf("Da 1 a 3 Bucchin\n");
    scanf("%d",&inputInt);
  }
  //Da questo punto in poi il Client si trova in gioco
  while(/*Partita ancora in corso){
    printGameGrid();
    movementRequest();
  }*/
  int sockfd;
  char c;
  struct sockaddr_un server_addr;
  if ((sockfd=socket(PF_LOCAL,SOCK_STREAM,0))<0) {
    printf("Errore apertura socket");
  }else{

    char add[]="/tmp/mio_socket5";
    server_addr.sun_family=AF_LOCAL;
    strcpy(server_addr.sun_path,add);

    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
      printf("Errore connessione socket\n");
    }else{
      while(read(sockfd,&c,1)>0){
        write(1,&c,1);
      }
    }
  }
  close(sockfd);
  return 0;
}
