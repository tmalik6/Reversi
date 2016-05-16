#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define uzenet_size 100

//**************************
int connect_szerver(int port);
void problem(char *message);
//******************************
int main(int argc, char **argv)
{
	int serverFD;
  	int port;
  	char port_number;
    int ok;
	
    char jovo_uzenet[uzenet_size]; //szerverkudli
    printf("Give me the port, where the server is running!\n");
	scanf("%s", &port_number);

	port = atoi(&port_number); // Konvert számmá

	serverFD = connect_szerver(port); // Csatlakozááá

    char elso_uzenet[uzenet_size];
    memset(elso_uzenet,'\0',uzenet_size);
    recv(serverFD, elso_uzenet,uzenet_size,0);
    printf("%s",elso_uzenet);
	
    if ((ok = uzenetek(serverFD)) < 1) {
    close(serverFD);
    exit(0);
    }
  
	return 0;
	
}
void problem(char *message) {
  printf("%s\n", message);
  exit(EXIT_FAILURE); //hogy tökéletesen portolható legyen, így jobb mint exit(0), ofcoursestackoverflow
}
int uzenetek(int serverFD)
{	
	char jovo_uzenet[uzenet_size]; //szerverkuldi
	char valasz[uzenet_size]; //clientkuldi
	
		 while(1)
    {
        memset(valasz,'\0',uzenet_size);
        printf("Mit Mondjak?\n");
        scanf("%s", valasz);         
        send(serverFD, valasz, uzenet_size, 0); 

        memset(jovo_uzenet,'\0',uzenet_size);
        recv(serverFD, jovo_uzenet,uzenet_size,0);
        if(strcmp(jovo_uzenet, "vege") == 0){
        printf("%s",jovo_uzenet); 
        break;  
        }
        else printf("%s",jovo_uzenet);
        
    }
    close(serverFD);
}
int connect_szerver(int port)
{
	int serverFD;

	struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); //bájtsorrendé át kell alakítani
    inet_aton("127.0.0.01", &(serverAddr.sin_addr)); //elméletileg ugyanazt csinálja mint az először
    memset(&(serverAddr.sin_zero),'\0',8); // a struktúra többi részének kinullázása

    if((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    	problem("Error Socket");
    	return -1;
    }//sock_stream-> tcp protokoll

    int yes=1;
    setsockopt(serverFD,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)); //újrahasználja a címet

     //innentől kezdve clientsként viselkedem

    if (connect(serverFD, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) == 1){
    	problem("Error: Connect");
    	return -1;
    }
    printf("Csatlakozva\n");

    return serverFD;
}
