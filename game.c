#include <stdio.h>


char field[8][8]; // Pálya 2D
char contener[64]; // Pálya 1D
int ponts[2];

void field_maker(char contener[]);
void init_field();
void convert();
void points_maker(int ponts[],char contener[]);
void set_data(int l, int c);
int insert();
void analysis_1();
void analysis_2();
void analysis_3();
void analysis_4();
void analysis_5();
void analysis_6();
void analysis_7();
void analysis_8();
void restart();
int next_round(); // swap

char home_char = 'X';
char away_char = 'O';

// globális i és j változom a 8 analizis függvényekhez
int i = 0;
int j = 0;

int point_counter = 0; // Ellenféltől hány bábút és pontot szerzek meg
int step = 0; // logikai változoként szerepel

int line = 0; 
int column = 0; 

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

	void init_field() {
	  int i, j;
	  for (i = 0; i < 8; ++i)
	    for (j = 0; j < 8; ++j)
	      field[i][j] = ':';

	  field[3][3] = home_char; //x
	  field[3][4] = away_char; //o
	  field[4][3] = away_char;
	  field[4][4] = home_char;
	}

	void convert() {
	  int i, j;
	  int elem = 0;
	  for (i = 0; i < 8; ++i)
	    for (j = 0; j < 8; ++j)
	      contener[elem++] = field[i][j];
	}
	int next_round() {
	  char temp = home_char;
	  home_char = away_char;
	  away_char = temp;
	  return 1;
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
	void set_data(int l, int c) {
  	line = l;
  	column = c;
	}
	int insert() {
  
	  analysis_1();
	  analysis_2();
	  analysis_3();
	  analysis_4();
	  analysis_5();
	  analysis_6();
	  analysis_7();
	  analysis_8();

  	return step; // Volt-e lépés
	}

void analysis_1() { // Adott poziciotol jobbra lévő elemek elfoglalásának tesztje, csak a "j"-vel lépkedek jobbra
  restart();
  for (j += 1; (j <= 7 && j >= 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == away_char); ++j)
    ++point_counter;
  if ((point_counter != 0) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int y;
    for ( y = column; y < j; ++y)
      field[i][y] = home_char;
  }
}
//**********************************************************************************************************//
//**********************************************************************************************************//

void analysis_2() { // Adott poziciotol balra lévő elemek elfoglalásának tesztje, csak a "j"-vel lépkedek balra
  restart();
  for (j -= 1; (j <= 7 && j >= 0) && (field[i][j] == away_char); --j)
    ++point_counter;
  if ((point_counter != 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int y;
    for (y = column; y > j; --y)
      field[i][y] = home_char;
  }
}
//**********************************************************************************************************//
//**********************************************************************************************************//

void analysis_3() { // Adott poziciotol felfelé(nagyobbtol a kisebbfelé) lévő elemek elfoglalásának tesztje, csak a "i"-vel lépkedek felfelé(csökken)
  restart();
  for (i -= 1; (i <= 7 && i >= 0) && (field[i][j] == away_char); --i)
    ++point_counter;
  if ((point_counter != 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int x;
    for (x = line; x > i; --x)
      field[x][j] = home_char;
   }
}
//**********************************************************************************************************//
//**********************************************************************************************************//

void analysis_4() { // Adott poziciotol lefelé(kisebbtol a nagyobbfelé) lévő elemek elfoglalásának tesztje, csak a "i"-vel lépkedek lefelé(nő)
  restart();
  for (i += 1; (i <= 7 && i >= 0) && (field[i][j] == away_char); ++i)
    ++point_counter;

  if ((point_counter != 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int x;
    for (x = line; x < i; ++x)
      field[x][j] = home_char;
   }
}
//**********************************************************************************************************//
//**********************************************************************************************************//

void analysis_5() { // Adott poziciotol jobbra fel(átló) lévő elemek elfoglalásának tesztje
  restart();
  for (i -= 1, j += 1; (i <= 7 && i >= 0) && (j <= 7 && j >= 0) && (field[i][j] == away_char); --i, ++j)
    ++point_counter;
  if ((point_counter != 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int x;
    int y;
    for (x = line, y = column; x > i, y < j ; --x, ++y)
      field[x][y] = home_char;
  }
}
//**********************************************************************************************************//
//**********************************************************************************************************//

void analysis_6() { // Adott poziciotol jobbra lefelé(átló) lévő elemek elfoglalásának tesztje
  restart();
  for (i += 1, j += 1; (i <= 7 && i >= 0) && (j <= 7 && j >= 0) && (field[i][j] == away_char); ++i, ++j)
    ++point_counter;

  if ((point_counter != 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int x,y;
    for (x = line, y = column; x < i, y < j ; ++x, ++y)
      field[x][y] = home_char;
  }
}
//**********************************************************************************************************//
//**********************************************************************************************************//

void analysis_7() { // Adott poziciotol balra le(átló) lévő elemek elfoglalásának tesztje
  restart();
  for (i += 1, j -= 1; (i <= 7 && i >= 0) && (j <= 7 && j >= 0) && (field[i][j] == away_char); ++i, --j)
    ++point_counter;

  if ((point_counter != 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int x,y;
    for (x = line, y = column; x < i, y > j ; ++x, --y)
      field[x][y] = home_char;
  }
}
//**********************************************************************************************************//
//**********************************************************************************************************//

void analysis_8() { // Adott poziciotol balra fel(átló) lévő elemek elfoglalásának tesztje
  restart();
  for (i -= 1, j -= 1; (i <= 7 && i >= 0) && (j <= 7 && j >= 0) && (field[i][j] == away_char); --i, --j)
    ++point_counter;

  if ((point_counter != 0) && !(i < 0 || i > 7 || j < 0 || j > 7) && (field[i][j] == home_char)) { // Ekkor átváltom az ő korongjait az enyémekre
    step = 1;
    int x,y;
    for (x = line, y = column; x > i, y > j ; --x, --y)
      field[x][y] = home_char;
   }
}
void restart() {
  i = line;
  j = column;
  point_counter = 0;
}