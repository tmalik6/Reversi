#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 512

//**********************************************************************************************************//
void print_board(char field[]);
void print_result(char result[]);
void problem(char *message);

int client(int port); 
int transmission(int gate); 
//**********************************************************************************************************//

int main(int argc, char **argv) {
  
  int gate;
  int port;
  char port_number;
  int readed;

  printf("Gimme the port, where the server is running!\n");
  scanf("%s", &port_number);

  port = atoi(&port_number); // Konvert számmá

  gate = client(port); // Csatlakozáá

  if ((readed = transmission(gate)) < 1) {
    close(gate);
    exit(EXIT_FAILURE); //hogy tökéletesen portolható legyen, így jobb mint exit(0), ofcoursestackoverflow
  }
  
  return 0;
}
//**********************************************************************************************************//

void print_board(char content[]) { 

  printf("\n\n\t\t\t\t R E V E R S I \n");
    printf("\t\t\t\t   The Game \n\n\n");

    printf("    ");
    int index = 0;
    int i = 1;
    for(i=1; i<=8; i++){
      printf("\t[%d] ", i);
    }
    printf("\n");
    
    for (i = 0; i < 8; i++)
    {
      printf("\t___");    
    }
    int j = 0;
    printf("\n");
    printf("\n");
    int szamok = 0;
    for (i = 0; i < 8; i++)
    {
      int jelzo = szamok+1;
      printf("  [%d]|",jelzo);

      for (j = 0; j < 8; ++j) {
          printf("\t%c", content[index++]); 
        } 

      printf("   |[%d]",jelzo);
      printf("\n\n");
      szamok = jelzo;
      jelzo = 0;
    }
    for (i = 0; i < 8; i++)
    {
      printf("\t___");    
    }
    printf("\n");
    for(i=1; i<=8; i++){
      printf("\t[%d] ", i);
    }
    printf("\n");
}
//**********************************************************************************************************//

void print_result(char content[]) { 
  int temp = 0;
  int result = 0;

  temp = content[0] - '0'; // 10-es helyiérték
  result += (10 * temp); 

  temp = content[1] - '0'; // 1-es helyiérték
  result += temp;
  printf("Home player's point(s) : %d\n", result);
  result = 0; 

  temp = content[3] - '0'; // 10-es helyiérték
  result += (10 * temp); 

  temp = content[4] - '0'; // 1-es helyiérték
  result += temp; 
  printf("Away player's point(s) : %d\n", result);  
}
//**********************************************************************************************************//

int transmission (int gate) {

  char buffer[BUFFER_SIZE]; // Szerver üzenete
  int reader; // Hiba kód
  char answer[BUFFER_SIZE]; // Kliens üzenete

  memset(buffer, 0, BUFFER_SIZE); // Buffer kinullázása
  if ((reader = read(gate, buffer, BUFFER_SIZE)) > 0) {
      if (strncmp(buffer, "welcome", 7) == 0) { 
      printf("Welcome\n");
      printf("Valid step format is : <y-coordinate>,<x-coordinate> --> y,x \n");
      printf("If you can't step : 'pass' \n");
      printf("If you want to give up : 'give_up'\n");
      printf("START\n");
      printf("Waiting...\n");
      transmission (gate);
      
    } else if (strncmp(buffer, "loose", 5) == 0) {
      printf("You loose!\n");
      write(gate, answer, BUFFER_SIZE);
      transmission(gate);
      
    } else if(strncmp(buffer, "winner", 6) == 0) {
      printf("You won!\n");
      write(gate, answer, BUFFER_SIZE);
      transmission(gate);
      
    } else if(strncmp(buffer, "draw", 4) == 0) {
      printf("Draw!\n");
      write(gate, answer, BUFFER_SIZE);
      transmission(gate);
      
    } else if(buffer[63] != '\0' && buffer[64] == '\0') { // pálya
      print_board(buffer);
      write(gate, answer, BUFFER_SIZE);
      transmission(gate);
      
    } else if(buffer[4] != '\0' && buffer[5] == '\0') { // eredmény
      print_result(buffer);
      write(gate, answer, BUFFER_SIZE);
      transmission(gate);
      
    } else if(strncmp(buffer, "active", 6) == 0) {
      printf("You turn:");
      scanf("%s", answer);
      write(gate, answer , BUFFER_SIZE);
      transmission(gate);

    } else if(strncmp(buffer, "passive", 7) == 0) {
      printf ("Waiting...\n");
      transmission (gate);
      
    } else if(strncmp(buffer, "invalid_step", 12) == 0) {
      printf("Invalid step!\n");
      printf("Try again: ");
      scanf("%s", answer);
      write(gate, answer, BUFFER_SIZE);
      transmission(gate);
      
    } else if(strncmp(buffer, "wrong_answer", 12) == 0) {
      printf("Wrong answer!\n");
      printf("Try again: ");
      scanf("%s", answer);
      write(gate, answer, BUFFER_SIZE);
      transmission (gate);

    } else {
      printf("%s\n", buffer);
      transmission(gate);
    }
    return 0;
  }
  return reader;
}
//**********************************************************************************************************//
void problem(char *message) {
  fprintf(stderr,"%s\n", message);
  exit(EXIT_FAILURE);
}
//**********************************************************************************************************//
int client (int port) {
  
  int gate;
  struct sockaddr_in server;

  server.sin_family = AF_INET; 
  server.sin_port = htons(port); //bájtsorrendé át kell alakítani

  inet_aton("127.0.0.1", &(server.sin_addr)); //ipv4, decimal -> 32bitnetwork byte
  memset(&(server.sin_zero), '\0', 8); // a struktúra többi részének kinullázása

  if ((gate = socket(PF_INET, SOCK_STREAM, 0)) == -1) { // Socket létrehozása
    problem("ERROR: Socket");
    return -1;
  }
  int yes=1;
  setsockopt(gate,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)); //újrahasználja a címet

  //innentől kezdve clientsként viselkedem
  
  if (connect(gate, (const struct sockaddr *) &server, sizeof(server)) == -1) { // Kapcsolódás kezdeményezése a socketen keresztül a szerverhez
    problem("ERROR: Connect");
    return -1;
    
  }
  printf("Connected!\n");
  return gate;
}
//**********************************************************************************************************//