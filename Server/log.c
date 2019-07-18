#include "log.h"

void substituteChar(char st[],char a, char b){
  int i=0;
  while(st[i]!='\0'){
    if(st[i]==a)
      st[i]=b;
    i++;
  }
  return;
}

void oraEsatta(char s[]){
  char buffer[26];
  time_t ora;
  time(&ora);
  strcpy(s,ctime_r(&ora, buffer));
  substituteChar(s,'\n','\0');
}

void LogServerStart(int *fdLog){

  char ora[26];
  char fileName[45]="File/Log/LOG_";
  int n_b_w;

  oraEsatta(ora);
  strcat(fileName,ora);
  substituteChar(fileName,' ' , '_');
  substituteChar(fileName,':','_');
  strcat(fileName,".txt");

  if((*fdLog=open(fileName,O_CREAT | O_APPEND | O_WRONLY,S_IRWXU))<0){
    /*Gestire cosa succede in caso di errore*/
  } else {
    if((n_b_w = write(*fdLog,ora,sizeof(ora))< sizeof(ora))){
      /*Gesire mancata scrittura su LOG*/
    } else {
      if((n_b_w = write(*fdLog,LOG_START_SERVER,sizeof(LOG_START_SERVER)-1)) < sizeof(LOG_START_SERVER)-1){
        /*Gesire mancata scrittura su LOG*/
      }
    }
  }
  return ;
}

void LogServerClose(int*fdLog){

  char ora[26];
  int n_b_w;

  oraEsatta(ora);

  if((n_b_w = write(*fdLog,ora,sizeof(ora))< sizeof(ora))){
      /*Gesire mancata scrittura su LOG*/
  } else {
    if((n_b_w = write(*fdLog,LOG_CLOSE_SERVER,sizeof(LOG_CLOSE_SERVER)-1)) < sizeof(LOG_CLOSE_SERVER)-1){
      /*Gesire mancata scrittura su LOG*/
    }
  }
  return ;
}
