#include <stdio.h>
#include <math.h>
//kolkokrzyzyk z inteligentnym przeciwnikiem

char who_plays_first(void);
char read_move(char);
char check_move_possibility(char, char[9]);
void save_move(char, char, char[9]);
void display_board(char[9]);
void fill_in_board(char[9]);
char computer_move(char[9], char);
char is_the_board_full(char[9]);
char did_somebody_win(char[9]);
void play_again(void);
void print_result(char[9], char);
char optimal_move(char[9], char);
char is_it_end(char[9]);

int main(void) {
   char board[9];
   char player = who_plays_first(); //funkcja losuje kto gra pierwszy
   char move;

   system("cls");
   fill_in_board(board); //wypelniamy tablice liczbami od 1 do 9
   do {
      if(player == 'x') {
         player = 'o'; //zmieniamy gracz
         display_board(board);
         do {
            move = read_move(player); //odczytujemy ruch gracza
         } while(!check_move_possibility(move, board));
      } else {
         player = 'x'; //zmieniamy gracza
         move = computer_move(board, player);
         system("cls");
         printf("Komputer wybral %d.\n\n", move + 1);
      }

      save_move(move, player, board);
   } while(!is_it_end(board)); //powtarzamy az do konca gry

   print_result(board, player);
   play_again();

   return 0;
}

char is_it_end(char board[9]) {
   char end = 0;

   if(did_somebody_win(board))
      end = 1;
   else if(is_the_board_full(board))
      end = 1;

   return end;
}

void print_result(char board[9], char player) {
   system("cls");
   display_board(board);

   if(did_somebody_win(board))
      printf("\n\nWygrywa gracz %c!", player);
   else
      printf("Remis!");
}

char who_plays_first(void) {
   char sign;

   srand(time(0)); //ustalanie roznych ziaren
   if(rand() % 2 == 1)
      sign = 'x';
   else
      sign = 'o'; //wybieramy ktory znak rusza pierwszy
   return sign;
}

char read_move(char player) {
   char move = 0;
   char move_char;

   do {
      printf("\n\nPodaj swoj ruch %c: ", player);
      while((move_char = getchar()) != '\n') { //pobieramy az do enter
         if((isdigit(move_char)) && (move == 0)) { //jesli znak to cyfra i nie odczytalismy jeszcze wartosci
            if(((move_char - '0') <= 9) && ((move_char - '0') >= 1)) //jesli wartosc cyfry w przedziale <1;9>
               move = move_char - '0'; //to zapisujemy jej wartosc
         }
      }
   } while(move == 0); //jesli w podanym ciagu nie bylo odpowiednich znakow, powtarzamy

   return move - 1;
}

char check_move_possibility(char move, char board[9]) {
   char is_it_possible = 1;

   if((board[move] == 'x') || (board[move] == 'o')) {
      is_it_possible = 0;
      printf("\nTo pole jest zajete!"); //komunikat
   }

   return is_it_possible; //zwracamy 0 jesli niemozliwy
}

void save_move(char move, char player, char board[9]) {
   board[move] = player;
}

void display_board(char board[9]) {
   char i;

   for(i = 0; i < 9; i++) {
      if(((i + 1) % 3 == 0) && (i + 1 != 9)) //jesli ostatni wyraz w linii i nie ostatni w ogole
         printf(" %c \n --------- \n", board[i]); //to linie poprzeczne
      else if(((i + 1) % 3 == 1) || (i + 1 == 9)) //jesli w pierwszej kolumnie albo ostatni w ogole
         printf(" %c ", board[i]); //to tylko zawartosc pola
      else
         printf("| %c |", board[i]); //a jesli na srodku to z belkami pionowymi
   }
}

void fill_in_board(char board[9]) {
   char i;

   for(i = 0; i < 9; i++) {
      board[i] = '1' + i;
   } //  od 1 do 9 
}

char is_the_board_full(char board[9]) {
   char i, result = 1;

   for(i = 0; i < 9; i++)
      if((board[i] <= '9') && (board[i] >= '0')) //jesli ktores pole jest cyfra
         result = 0; //to na pewno nie jest wypelnione

   return result;
}

char did_somebody_win(char board[9]) {
   char win = 0;
   char i;

   //sprawdzamy kolejne mozliwosci
   for(i = 0; i < 3; i++) {
      if((board[i] == board[i + 3]) && (board[i] == board[i + 6]))
         win = 1;
   }

   if(win == 0) {
      for(i = 0; i < 9; i = i + 3) {
         if((board[i] == board[i + 1]) && (board[i] == board[i + 2]))
            win = 1;
      }
   }

   if(win == 0) {
      if((board[0] == board[4]) && (board[0] == board[8]))
         win = 1;
      else if((board[2] == board[4]) && (board[4] == board[6]))
         win = 1;
   }

   return win;
}

void play_again(void) {
   char answer;
   char yes = 0;
   char no = 0;

   printf("\n\nGrasz ponownie?");
   printf("[T/N]");
   while((answer = getchar()) != '\n') //do nowej linii
   {
      if((answer == 'T') && (no == 0)) //jesli T i nie bylo odpowiedzi
         yes = 1;
      else if((answer == 'N') && (yes == 0)) //jesli nie i nie bylo odpowiedzi
         no = 1;
   }

   if(yes == 1)
      main();
   else if(no == 1)
      ;
   else
      play_again(); //powtarzamy funkcje jesli nie bylo odpowiedzi
}

char computer_move(char board[9], char player) { //wyznaczamy wg algorytmu min max
   char best_move, best_i_value, current_value, i;

   if(player == 'x') {
      current_value = -2; //domyslna wartosc -2
      for(i = 0; i < 9; i++) {
         if((board[i] != 'x') && (board[i] != 'o')) { //dla kazdego pustego pola
            board[i] = player; //ruch
            best_i_value = optimal_move(board, player); //wyznaczamy najmniejsza mozliwa wartosc ruchu
            board[i] = i + '1'; //cofamy ruch
            if(best_i_value > current_value) { //jesli ruch byl lepszy
               best_move = i; //aktualizujemy ruch
               current_value = best_i_value; //i aktualna wartosc
            }
         }
      }
   } else { //analogicznie, ale minimalizujemy
      current_value = 2; //domyslnie 2
      for(i = 0; i < 9; i++) {
         if((board[i] != 'x') && (board[i] != 'o')) {
            board[i] = player;
            best_i_value = optimal_move(board, player);
            board[i] = i + '1';
            if(best_i_value < current_value) {
               best_move = i;
               current_value = best_i_value;
            }
         }
      }
   }

   return best_move; //zwracamy najlepszy mozliwy ruch
}

char optimal_move(char board[9], char player) { //wyznaczanie najlepszej wartosci 
   char best_i_value, current_value, i;

   if(did_somebody_win(board)) { //sprawdzamy czy ktos wygral
      if(player == 'x')
         current_value = 1; //jesli tak to dla x bedzie 1
      else
         current_value = -1; //dla o -1
   } else if(is_the_board_full(board)) //remis
      current_value = 0; //zwracamy 0
   else {
      if(player == 'x')
         current_value = 2; //ustalamy wartosci odwrotnie (dla x 2, dla o -2)
      else
         current_value = -2; //bo pozniej zmienimy gracza
   }

   if(fabs(current_value) == 2) { //jesli nie bylo ani zwyciestwa ani remisu to szukamy najlepszego
      if(player == 'x') {
         player = 'o'; //zmieniamy
         for(i = 0; i < 9; i++) { //analogicznie jak wyzej
            if((board[i] != 'x') && (board[i] != 'o')) {
               board[i] = player;
               best_i_value = optimal_move(board, player);
               board[i] = i + '1';
               if(best_i_value < current_value) {
                  current_value = best_i_value;
               }
            }
         }
      } else {
         player = 'x';
         for(i = 0; i < 9; i++) {
            if((board[i] != 'x') && (board[i] != 'o')) {
               board[i] = player;
               best_i_value = optimal_move(board, player);
               board[i] = i + '1';
               if(best_i_value > current_value) {
                  current_value = best_i_value;
               }
            }
         }
      }
   }

   return current_value; //zwracamy najlepsza wartosc
}
