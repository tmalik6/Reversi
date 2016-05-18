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

#include "game.c"

#define uzenet_size 100
//********************************
int create_server(int port);
void problem(char *message);
//********************************

int main(int argc, char **argv) {
  
  if (argc != 2)
  problem("Így futtasd: ./server port_number\n");

  printf("Server is creating...\n");

  const char welcome[] = {"Üdvözöllek"};
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

  send(clientFD_1, welcome, sizeof(welcome) , 0);
  send(clientFD_2, welcome, sizeof(welcome) , 0);
  
  int lepes = 3; //lépésszámláló 1.->páros 2.->páratlan
  int pasz_1 = 0;
  int pasz_2 = 0;
  int vege = 2;
  init_field();
  
  field_maker(contener);


     while(1)
    {
        convert();
        if(pasz_1 == 1 && pasz_2 == 1)
        {
          break;
        }
        if(lepes % 2 == 1){ //1.es
          send(clientFD_2, passive, sizeof(passive), 0); //passivolom
          
          field_maker(contener);
          send(clientFD_1, contener, sizeof(contener) , 0); // Pályát megkapja az 1.játékos
          recv(clientFD_1, valasz, uzenet_size, 0);

          send(clientFD_1, active, sizeof(active), 0);              
          recv(clientFD_1, valasz, uzenet_size, 0);
          while(1)
          {
            if ((strncmp (valasz, "pasz", 4) == 0) && valasz[4] == '\0') { // Ha passzolt:

              printf("First player sent: pass\n");
              pasz_1 = 1; // 1-es player passzolt
              lepes++;
              break;
            }
            if ((strncmp (valasz, "give_up", 7) == 0) && valasz[7] == '\0') { // Ha feladta

               printf("First player sent: give_up\n");
               send(clientFD_2, winner, sizeof(winner) , 0);
               send(clientFD_1, loose, sizeof(loose), 0);
               vege = 1;
              break;
            }
            else if(valasz[1] == ',' && valasz[2] != '\0' && valasz[3] == '\0'){
              int sor = valasz[0] - '0'; //számmá alakítás
              int oszlop = valasz[2] - '0';//számmá alakítás koord
              --sor; //0...7
              --oszlop; //0...7
            
            if (line < 0 || column < 0 || line > 7 || column > 7 || field[line][column] != ':') {
            printf("1. player has an invalid step\n");
            send(clientFD_1, invalid_step, sizeof(invalid_step), 0);
            recv(clientFD_1, valasz, uzenet_size, 0);
            continue;
            }
            set_data(line, column);
            int sign = insert();

                if (sign) {
                   step = 0; 
                  lepes++;
                  next_round();
                  break;
                } else {
                  printf("1. player has an invalid step\n");
                  send(clientFD_1, invalid_step, sizeof(invalid_step), 0);
                  recv(clientFD_1, valasz, uzenet_size, 0);
                }
                } else {
                 printf("1. player has a wrong answer\n");
                 send(clientFD_1, wrong_answer, sizeof(wrong_answer), 0);
                 recv(clientFD_1, valasz, uzenet_size, 0);
                }            
          }

        }
        else  //2.es
        {
          send(clientFD_1, passive, sizeof(passive), 0); //passivolom
          
          field_maker(contener);
          send(clientFD_2, contener, sizeof(contener) , 0); // Pályát megkapja az 1.játékos
          recv(clientFD_2, valasz, uzenet_size, 0);

          send(clientFD_2, active, sizeof(active), 0);              
          recv(clientFD_2, valasz, uzenet_size, 0);

          while(1)
          {
            if ((strncmp (valasz, "pasz", 4) == 0) && valasz[4] == '\0') { // Ha passzolt:

              printf("Secound player sent: pass\n");
              pasz_2 = 1; // 1-es player passzolt
              lepes++;
              break;
            }
            if ((strncmp (valasz, "give_up", 7) == 0) && valasz[7] == '\0') { // Ha feladta

               printf("Secound player sent: give_up\n");
               send(clientFD_1, winner, sizeof(winner) , 0);
               send(clientFD_2, loose, sizeof(loose), 0);
               vege = 1;
              break;
            }
            else if(valasz[1] == ',' && valasz[2] != '\0' && valasz[3] == '\0'){
              int sor = valasz[0] - '0'; //számmá alakítás
              int oszlop = valasz[2] - '0';//számmá alakítás koord
              --sor; //0...7
              --oszlop; //0...7
            
            if (line < 0 || column < 0 || line > 7 || column > 7 || field[line][column] != ':') {
            printf("1. player has an invalid step\n");
            send(clientFD_2, invalid_step, sizeof(invalid_step), 0);
            recv(clientFD_2, valasz, uzenet_size, 0);
            continue;
            }
            set_data(line, column);
            int sign = insert();

                if (sign) {
                   step = 0; 
                  lepes++;
                  next_round();
                  break;
                } else {
                  printf("2. player has an invalid step\n");
                  send(clientFD_2, invalid_step, sizeof(invalid_step), 0);
                  recv(clientFD_2, valasz, uzenet_size, 0);
                }

                } else {
                 printf("2. player has a wrong answer\n");
                 send(clientFD_2, wrong_answer, sizeof(wrong_answer), 0);
                 recv(clientFD_2, valasz, uzenet_size, 0);
                 }
             if(vege == 1){  
              close(clientFD_1);
              close(clientFD_2);
              close(sock);
              return 0;
              }
          }          
        }
        if (vege == 1)
          {
            break;
          }  
    }
    close(clientFD_1);
    close(clientFD_2);
    close(sock);
    return 0;
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
