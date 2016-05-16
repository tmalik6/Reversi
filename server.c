#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#define uzenet_size 100
//********************************
int create_server(int port);
void problem(char *message);
//********************************

int main(int argc, char **argv) {
  
  if (argc != 2)
  problem("Így futtasd: ./server port_number\n");

  printf("Server is creating...\n");

  const char welcome[] = {"Üdvözzöllek"};
  const char active[] = {"Active"};
  const char passive[] = {"Passive"};
  const char invalid_step [] = {"Hibás lépés"};
  const char wrong_answer[] = {"Rossz válasz"};
  const char loose[] =   {"Vereség :("};
  const char winner[] = {"Győzelem :)"};
  const char draw[] =  {"Döntetlen :|"};

  int server_port = atoi(argv[1]);
  
  int sock = create_server(server_port);
  printf("Server is running...\n\n");

  struct sockaddr_in clientAddr;
  int sin_size = sizeof(struct sockaddr_in);
  int clientFD_1;
  int clientFD_2;
  

  printf("Várakozás a játékosokra...\n");
  clientFD_1 = accept(sock, (struct sockaddr *)&clientAddr, &sin_size);
  if(clientFD_1 == -1){problem("Error: In Player 1");}

  printf("Várakozás a második játékosra...\n");
  clientFD_2 = accept(sock, (struct sockaddr *)&clientAddr, &sin_size);
  if(clientFD_2 == -1){problem("Error: In Player 2");}  
  printf("Játék indulásra kész!\n");

  char uzenet[uzenet_size]; //szerverkudli
  char valasz[uzenet_size]; //clientkuldi

  memset(uzenet,'\0',100);
  strcpy(uzenet,"Welcome Player 1!\n");
  send(clientFD_1, uzenet, 100 , 0);

  memset(uzenet,'\0',100);
  strcpy(uzenet,"Welcome Player 2!\n");
  send(clientFD_2, uzenet, 100 , 0);
  
  
     while(1)
    {
        memset(valasz,'\0',uzenet_size);
        recv(clientFD_1, valasz,100, 0);
        printf("%s\n",valasz);
        fflush(stdout);
        if(strcmp(valasz, "vege") == 0){
          memset(uzenet,'\0',uzenet_size);
          strcpy(uzenet,"vege");
          send(clientFD_1, uzenet, 100 , 0);
          send(clientFD_2, uzenet, 100 , 0);
          break;
        } 

        memset(uzenet,'\0',uzenet_size);
        strcpy(uzenet,"vettem!\n");
        send(clientFD_1, uzenet, 100 , 0);

        memset(valasz,'\0',uzenet_size);
        recv(clientFD_2, valasz,100, 0);
        printf("%s\n",valasz);
        fflush(stdout);
        if(strcmp(valasz, "vege") == 0){
          memset(uzenet,'\0',uzenet_size);
          strcpy(uzenet,"vege");
          send(clientFD_1, uzenet, 100 , 0);
          send(clientFD_2, uzenet, 100 , 0);
          break;
        } 

        memset(uzenet,'\0',uzenet_size);
        strcpy(uzenet,"vettem!\n");
        send(clientFD_2, uzenet, 100 , 0);

        /*memset(uzenet,'\0',uzenet_size);
        recv(sock, uzenet,uzenet_size,0);
        printf("%s",uzenet);

        memset(uzenet,'\0',uzenet_size);
        strcpy(uzenet,"vettem!\n");
        send(clientFD_1, uzenet, 100 , 0);
        */

    }
    close(clientFD_1);
    close(clientFD_2);
    close(sock);

}
void problem(char *message) {
  printf("%s\n", message);
  exit(EXIT_FAILURE); //hogy tökéletesen portolható legyen, így jobb mint exit(0), ofcoursestackoverflow
}
create_server(int port) 
{
	int serverFD;
	serverFD = socket(AF_INET, SOCK_STREAM, 0);//sock_stream-> tcp protokoll

	if (serverFD == -1)
    problem("Error: Socket");

	struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); //bájtsorrendé át kell alakítani

    inet_aton("127.0.0.01", &(serverAddr.sin_addr)); //elméletileg ugyanazt csinálja mint az először
    memset(&(serverAddr.sin_zero),'\0',8); // a struktúra többi részének kinullázása

    int yes=1;
    setsockopt(serverFD,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)); //újrahasználja a címet
    //problem("Error: SetSockopt");

    if (bind(serverFD, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) == -1) //típuskényszeríteni kell a serverAddr-ezt
    problem("Error: Bind"); //socket hozzárendelése a hálózati címhez

  	if (listen(serverFD, 10) == -1) // Hallgatózás a socketen
    problem("Error: Listen");

	listen(serverFD, 10); //ellenőrizgetni az értékeket nem szabad kihagyni beadandóban

  return serverFD;
}
