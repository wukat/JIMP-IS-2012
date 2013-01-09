#include <stdio.h>
//kolkokrzyzyk z losowaniem


char who_plays_first(void);
char read_move(char);
char check_move_possibility(char, char[9]);
void save_move(char, char, char[9]);
void display_board(char[9]);
void fill_in_board(char[9]);
char computer_move(char[9]);
char is_the_board_full(char[9]);
char did_somebody_win(char[9]);
void play_again(void);
void print_result(char[9], char);
char is_it_end(char[9]);

int main(void)
{
	char board[9];
	char player = who_plays_first();  //funkcja losuje kto gra pierwszy
	char move;
	
	fill_in_board(board);	//wypelniamy tablice liczbami od 1 do 9
	do {
		if (player == 'x')  
		{
			player = 'o';  //zmieniamy gracz
			display_board(board);  //wyswietlamy
			do {
			  move = read_move(player);  //odczytujemy ruch gracza tak dlugo
		  } while (!check_move_possibility(move, board));  //az bedzie mozliwy
	  }
		else 
		{  
			player = 'x';  //zmieniamy gracza
			move = computer_move(board); //losujemy ruch komputera
		  system("cls");
		  printf("Komputer wybral %d.\n\n", move + 1);  //komunikat
		}
		
		save_move(move, player, board);		//zapisujemy ruch
		} while (!is_it_end(board)); //powtarzamy az do konca gry

  print_result(board, player);  //wyswietlamy wynik rozgrywki
  play_again();  //pytamy czy grac ponownie
  
  return 0;
}


char is_it_end(char board[9])
{
	char end = 0;
	
	if (did_somebody_win(board))
	  end = 1;
	else if(is_the_board_full(board))
	  end = 1;
	
	return end;
}


void print_result(char board[9], char player)
{
	system("cls");
	display_board(board);
	
	if (did_somebody_win(board))  //jesli ktos wygral 
	  printf("Wygrywa gracz %c!", player); //to komunikat kto
	else  //a jesli nie to na pewno remis
	  printf("Remis!");
}


char who_plays_first(void)  //kto ma pierwszy ruch (losowo)
{
	char sign; 
	
	srand(time(0));  //ustalanie roznych ziaren
	if (rand() % 2 == 1)
	  sign = 'x'; else sign = 'o'; //wybieramy ktory znak rusza pierwszy
	return sign;
}


char read_move(char player)  //funkcja odczytujaca jedna wspolrzedna ruchu z klawiatury; 
{
	char move = 0;   //wartosc zwracana
	char move_char;  //pobierane znaki
	
	do {   										//rob	
	  printf("\n\nPodaj swoj ruch %c: ", player);
    while ((move_char = getchar()) != '\n')		//pobieramy az do enter
	  {
			if ((isdigit(move_char)) && (move == 0))    //jesli znak to cyfra i nie odczytalismy jeszcze wartosci
		  {
			  if (((move_char - '0') <= 9) && ((move_char - '0') >= 1))  //jesli wartosc cyfry w przedziale <1;9>
			    move = move_char - '0';  //to zapisujemy jej wartosc
		  }
	  }
  } while (move == 0);  //jesli w podanym ciagu nie bylo odpowiednich znakow, powtarzamy
  
  return move - 1;  //zwaracamy wartosc koordynatu
}


char check_move_possibility(char move, char board[9])   //sprawdzanie czy ruch jest mozliwy
{
	char is_it_possible = 1;  //wartosc logiczna (bez boola)
	
	if ((board[move] == 'x') || (board[move] == 'o'))  //jesli wartosc pola > 0 (poczatkowa zawsze 0)
	{
		is_it_possible = 0;   //to pole zajete, ruch niemozliwy
		printf("\nTo pole jest zajete!");  //komunikat
	}
	
	return is_it_possible;  //zwracamy czy ruch mozliwy
}


void save_move(char move, char player, char board[9])  //zapisywanie ruchu
{
	board[move] = player;
}

void display_board(char board[9])  //wyswietlanie tablicy
{
	char i;
	
	for (i = 0; i < 9; i++)  //petla 
  {
		if (((i + 1) % 3 == 0) && (i + 1 != 9))  //jesli ostatni wyraz w linii i nie ostatni w ogole
		  printf(" %c \n --------- \n", board[i]);  //to linie poprzeczne
		else if (((i + 1) % 3 == 1) || (i + 1 == 9))  //jesli w pierwszej kolumnie albo ostatni w ogole
		  printf(" %c ", board[i]);  //to tylko zawartosc pola
		else
		  printf("| %c |", board[i]);  //a jesli na srodku to z belkami pionowymi
	}	
}

void fill_in_board(char board[9])  //wypelniamy tablice
{
	char i;
	
	for (i = 0; i < 9; i++)
	{
		board[i] = '1' + i;  //numerujemy pola
	}
}

char is_the_board_full(char board[9])  //sprawdzamy czy wszystkie pola zapelnione
{
	char i, result = 1;
	
	for (i = 0; i < 9; i++)
	  if ((board[i] <= '9') && (board[i] >= '0'))  //jesli ktores pole jest cyfra
	  result = 0;  //to nie sa wypelnione
	      
	return result;  
}


char did_somebody_win(char board[9])  //czy ktos wygral
{
	char win = 0;
	char i;
	//sprawdzamy kolejne mozliwosci
	for (i = 0; i < 3; i++)
  {
		if ((board[i] == board[i + 3]) && (board[i] == board[i + 6]))
		win = 1;
	}
		
	if (win == 0)
	{
		for (i = 0; i < 9; i = i + 3)
    {
	    if ((board[i] == board[i + 1]) && (board[i] == board[i + 2]))
	    win = 1;
	  }
  }
	
	if (win == 0)
	{
		if ((board[0] == board[4]) && (board[0] == board[8]))
	  win = 1;
	  else if ((board[2] == board[4]) && (board[4] == board[6]))
	  win = 1;
	}

	return win;	
}


char computer_move(char board[9])  //losowanie ruchu komputera
{
	char move; 
	
	do {
	  srand(time(0));  //ziarno
	  move = rand() % 9;  //losujemy 0-8
	} while ((board[move] == 'x') || (board[move] == 'o'));  //dopoki ruch nie bedzie mozliwy
	
	return move;	 
}


void play_again(void)  //czy ponownie
{
	char answer; //zmienna na znaki
	char yes =0 ;
	char no = 0;  //flagi dla tak i nie
		
	printf("\n\nGrasz ponownie?");   //pytamy
	printf("[T/N]");  //odpowiedzi
	while ((answer = getchar()) != '\n')  //petla nieskonczona
	{
	  if ((answer == 'T') && (no == 0)) //jesli T i nie bylo odpowiedzi
	    yes = 1;  //to tak
	  else if ((answer == 'N') && (yes == 0))   //jesli nie i nie bylo odpowiedzi
		  no = 1;  //to nie
  }
  
  if (yes == 1)  //tak 
    main(); //to odpalamy gre ponownie
  else if (no == 1)  //nie
    ;  //to przerywamy petle i konczy sie funkcja
  else 
	  play_again();  //powtarzamy funkcje jesli nie bylo odpowiedzi
}
