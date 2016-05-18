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


char contener[64]; // Pálya 1D
int ponts[2];
char home_char = 'X';
char away_char = 'O';
//*********************
void field_maker(char contener[]);
void points_maker(int ponts[],char contener[]);
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
    printf("Gimme the port, where the server is running!\n");
	scanf("%s", &port_number);

	port = atoi(&port_number); // Konvert számmá

	serverFD = connect_szerver(port); // Csatlakozááá
	
    if ((ok = uzenetek(serverFD)) < 1) {
    close(serverFD);
    exit(EXIT_FAILURE);
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
	int ok;
    memset(jovo_uzenet, 0, uzenet_size);
    if((ok = recv(serverFD,jovo_uzenet,uzenet_size,0)) > 0)
    {
        if(strncmp(jovo_uzenet, "Üdvözöllek",10) == 0)
        {   
            memset(jovo_uzenet,'\0',uzenet_size);
            printf("Üdvözöllek\n");
            printf("Helyes lépési formátum: x-koordináta,y-koordináta -> x,y\n");
            printf("Ha nem tudsz lépni: pasz\n");
            printf("Ha fel akarod adni: give_up\n");
            printf("Kezdődik!!!\n");
            uzenetek(serverFD);
        }
        else if(strncmp(jovo_uzenet, "Vereség :(", 10) == 0)
        {
            printf("Vereség :(\n");
            send(serverFD, valasz, uzenet_size, 0);
            uzenetek(serverFD);
        }
        else if(strncmp(jovo_uzenet, "Győzelem :)", 11) == 0)
        {
            printf("Győzelem :)\n");
            send(serverFD, valasz, uzenet_size, 0);
            uzenetek(serverFD);
        }
        else if(strncmp(jovo_uzenet, "Döntetlen :|", 12) == 0)
        {
            printf("Döntetlen :|\n");
            send(serverFD, valasz, uzenet_size, 0);
            uzenetek(serverFD);
        }
        else if(strncmp(jovo_uzenet, "Rossz válasz", 10) == 0)
        {
            printf("Rossz válasz\n");
            printf("Adjon meg újat:\n");
            scanf("%s",valasz);
            send(serverFD, valasz, uzenet_size, 0);
            uzenetek(serverFD);
        }
        else if(strncmp(jovo_uzenet, "Hibás lépés", 11) == 0)
        {
            printf("Hibás lépés\n");
            printf("Adjon meg újat:\n");
            scanf("%s",valasz);
            send(serverFD, valasz, uzenet_size, 0);
            uzenetek(serverFD);
        }
         else if(jovo_uzenet[63] != '\0' && jovo_uzenet[64] == '\0') { // pálya
            field_maker(jovo_uzenet);
            points_maker(ponts, jovo_uzenet);
            send(serverFD, valasz, uzenet_size, 0);
            printf("1es játékos pontjai: %d\n",ponts[0]);
            printf("2es játékos pontjai: %d\n",ponts[1]);
            uzenetek(serverFD);
        }  
        else if(strncmp(jovo_uzenet, "Active", 6) == 0) {              
            printf("You turn:");
            scanf("%s", valasz);
            send(serverFD, valasz , uzenet_size, 0);
            uzenetek(serverFD);
        } 
        else if(strncmp(jovo_uzenet, "Passive", 7) == 0) {
            printf ("Várakozás a másik játékosra...\n");
            uzenetek(serverFD);
        }
        else{
            printf("%s\n", jovo_uzenet);
            uzenetek(serverFD);        
        } 
        return 0;
    }
    return ok;
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
void field_maker(char contener[]){
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
            printf("\t%c", contener[index++]); 
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
    void points_maker(int ponts[], char contener[])
    {
        //0.as az 1es játékos
        //1.es a  2es játékos
        int i = 0;
        int egyeske = 0; 
        int ketteske = 0;
        for (i = 0; i < 64; i++)
        {
            if(contener[i] == home_char) {
                egyeske++;
            }
            else if(contener[i] == away_char){
                ketteske++;
            }
        }
        ponts[0] = egyeske;
        ponts[1] = ketteske;
    }