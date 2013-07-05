#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jimp.h"
#define duzo 10000 

int ** dijkstra(int ** odleglosci, int rozmiar, int wierzcholek) {
   int i, j, index, indeks, minimalny, dlugosc_Q, **Q, **sasiedzi, **najkrotsze = NULL;

   if((najkrotsze = malloc(rozmiar * sizeof(int *))) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   }
   for(i = 0; i < rozmiar; i++)
      if((najkrotsze[i] = malloc(3 * sizeof(int))) == NULL) {
         printf("Blad alokacji pamieci");
         exit(1);
      }

   /*inicjalizacja*/
   for(i = 0; i < rozmiar; i++) {
      najkrotsze[i][0] = odleglosci[wierzcholek][i]; /*najkrotsze[i][0] - odleglosci od 'wierzcholek'*/
      if(najkrotsze[i][0] != duzo)
         najkrotsze[i][1] = wierzcholek; /*najkrotsze[i][1] - poprzedniki*/
      else
         najkrotsze[i][1] = -1;
      najkrotsze[i][2] = i; /*najkrotsze[i][2] - indeksy*/
   }

   if((sasiedzi = malloc(rozmiar * sizeof(int*))) == NULL) { /*tablica list sasiadow*/
      printf("Blad alokacji pamieci");
      exit(1);
   }
   for(i = 0; i < rozmiar; i++)
      if((sasiedzi[i] = tworz_liste_sasiadow(odleglosci, rozmiar, i)) == NULL) {
         printf("Blad alokacji pamieci");
         exit(1);
      }

   if((Q = malloc(2 * sizeof(int*))) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   }
   if((Q[0] = malloc(rozmiar * sizeof(int))) == NULL) { /*indeksy*/
      printf("Blad alokacji pamieci");
      exit(1);
   }
   if((Q[1] = malloc(rozmiar * sizeof(int))) == NULL) {/*wartosci*/
      printf("Blad alokacji pamieci");
      exit(1);
   }
   for(i = 0; i < rozmiar; i++) {
      Q[1][i] = odleglosci[wierzcholek][i];
      Q[0][i] = i;
   }

   dlugosc_Q = rozmiar;
   while(Q != NULL) {
      index = indeks_minimum_tablicy(Q, dlugosc_Q);
      for(j = 0; j < dlugosc_Q; j++) {
         if(Q[0][j] == index)
            indeks = j;
      }
      minimalny = Q[1][indeks];
      Q = przepisz(Q, index, dlugosc_Q);
      dlugosc_Q--;
      i = 0;
      while(sasiedzi[index][i] != -1) {
         if(najkrotsze[sasiedzi[index][i]][0] > minimalny + odleglosci[index][sasiedzi[index][i]]) {
            najkrotsze[sasiedzi[index][i]][0] = minimalny + odleglosci[index][sasiedzi[index][i]];
            najkrotsze[sasiedzi[index][i]][1] = index;
            for(j = 0; j < dlugosc_Q; j++) {
               if(Q[0][j] == sasiedzi[index][i])
                  indeks = j;
            }
            Q[1][indeks] = najkrotsze[sasiedzi[index][i]][0];
         }
         i++;
      }
   }

   najkrotsze[wierzcholek][0] = 0;
   najkrotsze[wierzcholek][1] = wierzcholek;

   for(i = 0; i < rozmiar; i++)
      free(sasiedzi[i]);
   free(sasiedzi);

   najkrotsze = sortuj(najkrotsze, rozmiar);
   return najkrotsze;
}

int ** przepisz(int **tablica, int indeks_usuwany, int rozmiar) {
   int j, indeks;

   for(j = 0; j < rozmiar; j++) {
      if(tablica[0][j] == indeks_usuwany)
         indeks = j;
   }

   for(j = indeks; j < rozmiar - 1; j++) {
      tablica[0][j] = tablica[0][j + 1];
      tablica[1][j] = tablica[1][j + 1];
   }

   if(rozmiar == 1) {
      tablica[0] = NULL;
      tablica[1] = NULL;
      tablica = NULL;
   } else {
      if((tablica[0] = realloc(tablica[0], (rozmiar - 1) * sizeof(int))) == NULL) {
         printf("Blad alokacji pamieci");
         exit(1);
      }
      if((tablica[1] = realloc(tablica[1], (rozmiar - 1) * sizeof(int))) == NULL) {
         printf("Blad alokacji pamieci");
         exit(1);
      }
   }

   return tablica;
}

int * tworz_liste_sasiadow(int ** odleglosci, int rozmiar, int indeks) {
   int i, dlugosc_listy, *lista_sasiadow = NULL;

   dlugosc_listy = 1;
   if((lista_sasiadow = realloc(lista_sasiadow, dlugosc_listy)) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   };
   lista_sasiadow[dlugosc_listy - 1] = -1;
   for(i = 0; i < rozmiar; i++) {
      if(odleglosci[indeks][i] != duzo) {
         if((lista_sasiadow = realloc(lista_sasiadow, (dlugosc_listy + 1) * sizeof(int))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         };
         lista_sasiadow[dlugosc_listy - 1] = i;
         lista_sasiadow[dlugosc_listy] = -1; /*oznaczamy ostatni element, zeby nie przekazywac rozmiaru listy*/
         dlugosc_listy++;
      }
   }

   return lista_sasiadow;
}

int indeks_minimum_tablicy(int ** tablica, int rozmiar) {
   int indeks, mini, i;

   mini = tablica[1][0];
   indeks = tablica[0][0];

   for(i = 1; i < rozmiar; i++) {
      if(tablica[1][i] < mini) {
         mini = tablica[1][i];
         indeks = tablica[0][i];
      }
   }

   return indeks;
}

int ** sortuj(int ** najkrotsze, int rozmiar) {
   int i, j, zamiana = 1;
   int * tymczasowy;

   if((tymczasowy = malloc(3 * sizeof(int))) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   }

   j = 0;
   while(zamiana) {
      zamiana = 0;
      for(i = 0; i < rozmiar - j - 1; i++) {
         if(najkrotsze[i][0] > najkrotsze[i + 1][0]) {
            tymczasowy = najkrotsze[i];
            najkrotsze[i] = najkrotsze[i + 1];
            najkrotsze[i + 1] = tymczasowy;
            zamiana = 1;
         }
      }
      j++;
   }

   return najkrotsze;
}

int znajdz_odleglosc(int ** najkrotsze, int rozmiar, int wierzcholek) {
   int i, odleglosc = -1;

   for(i = 0; i < rozmiar; i++)
      if(najkrotsze[i][2] == wierzcholek)
         odleglosc = najkrotsze[i][0];

   if(odleglosc == -1) {
      printf("Blad w danych!");
      exit(1);
   }

   return odleglosc;
}
