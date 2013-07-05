#include <stdio.h>
#include <string.h>

int make_pesel(char pesel[]) { //przerobienie stringa na chary z odpowienimi wartosciami (jak bylo '9' to teraz jest 9)
   int i, error;

   error = 0;
   for(i = 0; i < 11; i++)
      if((pesel[i] > 47) && (pesel[i] < 58)) //sprawdzenie czy wprowadzone znaki to cyfry
         pesel[i] -= 48; //przerobienie '9' na 9
      else
         error = 1; //jesli zle to przerywamy

   return error;
}

int check(char pesel[]) { //sprawdza znak kontrolny
   int check, error;

   error = 0;
   check = (1 * pesel[0] + 3 * pesel[1] + 7 * pesel[2] + 9 * pesel[3] + 1 * pesel[4] + 3 * pesel[5] + 7 * pesel[6]
         + 9 * pesel[7] + 1 * pesel[8] + 3 * pesel[9]) % 10; //modulo (%) 10

   if(10 - check != pesel[10]) //jesli nie ok, tzn liczba kontrolna to nie 10 - check
      error = 1; // to blad

   return error;
}

int make_and_check_date(char pesel[], int date[]) { //wyciagamy date z peselu
   int error; //tablica na rok, miesiac, dzien

   error = 0;
   date[0] = pesel[0] * 10 + pesel[1]; //rok z peselu, pierwsza cyfra dziesietna, druga jednosci
   date[1] = pesel[2] * 10 + pesel[3]; // miesiac
   date[2] = pesel[4] * 10 + pesel[5]; // dzien

   //wg odpowiedniego algorytmu tworzenia peselu (do numeru miesiaca dodawane sa odpowiednie wartosci
   //w zaleznosci od stulecia
   if(date[2] <= 31) { //sprawdzamy zakres dni
      if((date[1] >= 81) && (date[1] <= 92)) {
         date[1] -= 80;
         date[0] += 1800;
      } else if((date[1] >= 1) && (date[1] <= 12))
         date[0] += 1900;
      else if((date[1] >= 21) && (date[1] <= 32)) {
         date[1] -= 20;
         date[0] += 2000;
      } else if((date[1] >= 41) && (date[1] <= 52)) {
         date[1] -= 40;
         date[0] += 2100;
      } else if((date[1] >= 61) && (date[1] <= 72)) {
         date[1] -= 60;
         date[0] += 2200;
      } else
         error = 1; //to blad

      if((date[2] > 29) && (error == 0)) { //sprawdzamy dlugosc miesiecy
         if(date[1] == 2) //jesli luty i dni >29
            error = 1; //to blad (pomijam lata nieprzestepne)
         else if(date[2] == 31) //jesli dni 31
            if((date[1] == 4) || (date[1] == 6) || (date[1] == 9) || (date[1] == 11)) //a miesiac ma 30
               error = 1; //to blad
      }
   } else
      //jesli dni >31
      error = 1; //to tym bardziej blad

   return error; //zwracamy wartosc bledu
}

char check_sex(char pesel[], char name[], int *sex) { //sprawdzamy plec
   int error;

   error = 0;
   if(((name[strlen(name) - 1] == 'a') && (pesel[9] % 2 == 0)) && (strcmp(name, "Bonawentura") != 0)) //jesli kobieta, to imie konczy sie na 'a', 10 cyfra parzysta, imie != od Bonawentura
      *sex = 1; //ustalamy plec = kobieta
   else if(((name[strlen(name) - 1] != 'a') && (pesel[9] % 2 == 1)) && (strcmp(name, "Bonawentura") != 0)) //dla mezczyzn (nie bonawentury)
      *sex = 0; //mezczyzna
   else if(((name[strlen(name) - 1] == 'a') && (pesel[9] % 2 == 1)) && (strcmp(name, "Bonawentura") == 0)) //poprawne dla Bonawentury
      *sex = 0; //mezczyzna
   else
      error = 1; // jesli zaden to blad

   return error; //zwracamy blad
}

void print_results(int date[], char pesel[], char name[], int *sex) { //drukujemy na ekran
   int i;

   printf("%s, Twoj PESEL jest poprawny: ", name); //mamy wydruk: imie,
   for(i = 0; i < 11; i++)
      printf("%d", pesel[i]); // pesel
   if(*sex == 1)
      printf("\nTwoja plec: kobieta.");
   else
      printf("\nTwoja plec: mezczyzna.");
   printf("\nTwoja data urodzenia: %02d.%02d.%4d.", date[2], date[1], date[0]); //data urodzenia
}

int main(void) {
   //deklaracja zmiennych, tablice sa globalne
   char pesel[12], name[30];
   int date[3], error, sex;

   error = 0;
   sex = 0;

   // wczytanie danych
   printf("Podaj swoje imie: \n");
   scanf("%s", name);
   printf("Czesc %s, wpisz swoj PESEL. \n", name);
   scanf("%s", pesel);

   error = make_pesel(pesel); //kazda funkcja zwraca albo 0 albo 1
   if(error == 0) {
      error = check(pesel);
      if(error == 0) {
         error = make_and_check_date(pesel, date);
         if(error == 0) {
            error = check_sex(pesel, name, &sex);
            if(error == 0)
               print_results(date, pesel, name, &sex);
            else
               printf("Twoja plec nie zgadza sie z imieniem!");
         } else
            printf("Bledna data urodzenia");
      } else
         printf("Blad liczby kontrolnej!");
   } else
      printf("Bledny numer PESEL!");

   return 0; //no
}
