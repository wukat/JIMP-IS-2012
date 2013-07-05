#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jimp.h"

/*--------------dzialania na datach----------------*/
/*dla uproszczenia przyjmuje, ze rok ma 365, a miesiac 30 dni*/
int roznica_dat(data a, data b) {
   int roznica;

   roznica = (a.rok - b.rok) * 365 + (a.miesiac - b.miesiac) * 30 + (a.dzien - b.dzien);

   return roznica;
}

data dodaj_do_daty(data a, int n) {
   a.dzien = a.dzien + n;
   if(a.dzien > 30) {
      a.dzien = a.dzien % 30;
      a.miesiac = a.miesiac + a.dzien / 30;
      if(a.miesiac > 12) {
         a.miesiac = a.miesiac % 12;
         a.rok = a.rok + a.miesiac / 12;
      }
   }

   return a;
}
/*--------------------------------------------*/

FILE * otworz_plik(char * katalog, char * nazwa_pliku, char * tryb) {
   char * sciezka = NULL;
   FILE * plik;

   if((sciezka = malloc((strlen(katalog) + 2 + strlen(nazwa_pliku)) * sizeof(char))) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   }
   strcpy(sciezka, katalog);
   strcat(sciezka, "/");
   strcat(sciezka, nazwa_pliku);
   if((plik = fopen(sciezka, tryb)) == NULL) {
      perror("Blad otwarcia pliku");
      exit(1);
   }
   free(sciezka);

   return plik;
}

int szukaj_indeksu_miasta(char *miasto, char ** miasta, int rozmiar) {
   int index, i;
   index = -1;

   for(i = 0; i < rozmiar; i++) {
      if(strncmp(miasta[i], miasto, strlen(miasta[i]) - 1) == 0)
         index = i;
   }

   if(index == -1) {
      printf("Blad! W plikach znajduja sie bledne dane!");
      exit(1);
   }

   return index;
}

long int znajdz_ID(artykul * artykuly, int liczba_artykulow, char * nazwa) {
   int i;
   long int ID = -1;

   for(i = 0; i < liczba_artykulow; i++) {
      if(strncmp(artykuly[i].nazwa, nazwa, strlen(artykuly[i].nazwa) - 1) == 0)
         ID = artykuly[i].ID;
   }

   if(ID == -1) {
      printf("Blad! W plikach znajduja sie bledne dane!");
      exit(1);
   }

   return ID;
}

char * znajdz_nazwe(artykul * artykuly, int liczba_artykulow, long int ID) {
   int i;
   char * nazwa = NULL;

   for(i = 0; i < liczba_artykulow; i++) {
      if(artykuly[i].ID == ID) {
         nazwa = malloc((strlen(artykuly[i].nazwa) + 1) * sizeof(char));
         strcpy(nazwa, artykuly[i].nazwa);
      }
   }

   if(nazwa == NULL) {
      printf("Blad! W plikach znajduja sie bledne dane!");
      exit(1);
   }

   return nazwa;
}

