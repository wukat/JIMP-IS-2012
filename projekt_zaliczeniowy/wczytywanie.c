#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jimp.h"
#define duzo 10000

char** lista_miast(int *rozmiar, char * katalog) {
   FILE * miejscowosci;
   char nazwa[40];
   int i;
   char ** miasta = NULL;

   miejscowosci = otworz_plik(katalog, "miejscowosci.dat", "r");

   i = 0;
   while(fgets(nazwa, 40, miejscowosci)) {
      if(nazwa[0] > '9') {
         if((miasta = realloc(miasta, (i + 1) * sizeof(char *))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         if((miasta[i] = malloc((strlen(nazwa) + 1) * sizeof(char))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         strcpy(miasta[i], nazwa);
         i++;
      }
   }
   *rozmiar = i;
   fclose(miejscowosci);

   return miasta;
}

int ** macierz_odleglosci(int rozmiar, char ** miasta, char * katalog) {
   FILE * miejscowosci;
   char nazwa[40];
   int i, j, odleglosc;
   int ** odleglosci = NULL;

   miejscowosci = otworz_plik(katalog, "miejscowosci.dat", "r");

   if((odleglosci = malloc(rozmiar * sizeof(int *))) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   }
   for(i = 0; i < rozmiar; i++)
      if((odleglosci[i] = malloc(rozmiar * sizeof(int))) == NULL) {
         printf("Blad alokacji pamieci");
         exit(1);
      }
   for(i = 0; i < rozmiar; i++)
      for(j = 0; j < rozmiar; j++)
         odleglosci[i][j] = duzo;

   while(feof(miejscowosci) == 0) {
      odleglosc = 0;
      fscanf(miejscowosci, "%d", &odleglosc);
      if(odleglosc > 0) {
         fgetc(miejscowosci);
         fgets(nazwa, 40, miejscowosci);
         j = szukaj_indeksu_miasta(nazwa, miasta, rozmiar);
         odleglosci[i][j] = odleglosc;
         odleglosci[j][i] = odleglosc;
      } else {
         fgets(nazwa, 40, miejscowosci);
         i = szukaj_indeksu_miasta(nazwa, miasta, rozmiar);
      }
   }
   fclose(miejscowosci);

   return odleglosci;
}

artykul * lista_artykulow(int * liczba_artykulow, char * katalog) {
   FILE * artykuly;
   char nazwa[80];
   long int ID;
   int i;
   artykul * spis = NULL;

   artykuly = otworz_plik(katalog, "artykuly.dat", "r");

   i = 0;
   while(feof(artykuly) == 0) {
      fscanf(artykuly, "%ld ", &ID);
      fgets(nazwa, 80, artykuly);
      if((spis = realloc(spis, (i + 1) * sizeof(artykul))) == NULL) {
         printf("Blad alokacji pamieci");
         exit(1);
      }
      spis[i].ID = ID;
      if((spis[i].nazwa = malloc((strlen(nazwa) + 1) * sizeof(char))) == NULL) {
         printf("Blad alokacji pamieci");
         exit(1);
      }
      strcpy(spis[i].nazwa, nazwa);
      i++;
   }
   free(spis[i - 1].nazwa);
   if((spis = realloc(spis, (i - 1) * sizeof(artykul))) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   }

   *liczba_artykulow = i - 1;
   fclose(artykuly);

   return spis;
}

hurtownia * lista_hurtowni(artykul * artykuly, int l_artykulow, int * l_hurtowni, char ** miasta, int l_miast,
      char * katalog) {
   FILE * hurtownie;
   char nazwa[80];
   long int ID;
   int ilosc, i, j;
   float cena;
   data termin;
   hurtownia * spis = NULL;

   hurtownie = otworz_plik(katalog, "hurtownie.dat", "r");

   i = 0;
   while(feof(hurtownie) == 0) {
      ID = 0;
      fscanf(hurtownie, "%ld ", &ID);
      if(ID == 0) {
         fgets(nazwa, 80, hurtownie);
         if((spis = realloc(spis, (i + 1) * sizeof(hurtownia))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         if((spis[i].miasto = malloc((strlen(nazwa) + 1) * sizeof(char))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         strcpy(spis[i].miasto, nazwa);
         spis[i].produkty = NULL;
         spis[i].numer_miasta = szukaj_indeksu_miasta(spis[i].miasto, miasta, l_miast);
         i++;
         j = 0;
      } else {
         fscanf(hurtownie, "%d ", &ilosc);
         fscanf(hurtownie, "%f ", &cena);
         fscanf(hurtownie, "%d ", &termin.dzien);
         fscanf(hurtownie, "%d ", &termin.miesiac);
         fscanf(hurtownie, "%d ", &termin.rok);
         if((spis[i - 1].produkty = realloc(spis[i - 1].produkty, (j + 1) * sizeof(sklad))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         spis[i - 1].produkty[j].ID = ID;
         spis[i - 1].produkty[j].nazwa = znajdz_nazwe(artykuly, l_artykulow, ID);
         spis[i - 1].produkty[j].cena = cena;
         spis[i - 1].produkty[j].ilosc = ilosc;
         spis[i - 1].produkty[j].waznosc = termin;
         j++;
         spis[i - 1].ilosc_pozycji = j;
      }
   }

   *l_hurtowni = i;

   spis = posortuj_po_datach(spis, *l_hurtowni);
   fclose(hurtownie);

   return spis;
}

/*
 Wewnatrz kadej hurtowni posortuje produkty od najkblizszej do najdalszej daty waznosci, pozwoli to zawsze brac produtkr o
 krotszej przydatnosci (jesli to mozliwe) i usprwnic usuwanie przeterminowanych.
 */
hurtownia * posortuj_po_datach(hurtownia * hurtownie, int liczba_hurtowni) {
   int i, j, k, zamiana;
   sklad tymczasowy;

   for(i = 0; i < liczba_hurtowni; i++) {
      zamiana = 1;
      k = 1;
      while(zamiana) {
         zamiana = 0;
         for(j = 0; j < hurtownie[i].ilosc_pozycji - k; j++) {

            if(((roznica_dat(hurtownie[i].produkty[j].waznosc, hurtownie[i].produkty[j + 1].waznosc) > 0)
                  || ((hurtownie[i].produkty[j].waznosc.dzien == 0) && (hurtownie[i].produkty[j].waznosc.miesiac == 0)
                        && (hurtownie[i].produkty[j].waznosc.rok == 0)))
                  && !((hurtownie[i].produkty[j + 1].waznosc.dzien == 0)
                        && (hurtownie[i].produkty[j + 1].waznosc.miesiac == 0)
                        && (hurtownie[i].produkty[j + 1].waznosc.rok == 0))) {
               tymczasowy = hurtownie[i].produkty[j];
               hurtownie[i].produkty[j] = hurtownie[i].produkty[j + 1];
               hurtownie[i].produkty[j + 1] = tymczasowy;
               zamiana = 1;
            }
         }
      }
      k++;
   }

   return hurtownie;
}

zamowienie * lista_zamowien(char ** miasta, int liczba_miast, artykul * artykuly, int liczba_artykulow,
      int * liczba_zamowien, char * katalog) {
   FILE * zamowienia;
   char nazwa[80];
   int numer, i, j;
   data termin;
   zamowienie * spis = NULL;

   zamowienia = otworz_plik(katalog, "zlecenie.dat", "r");

   i = 0;
   while(feof(zamowienia) == 0) {
      termin.rok = -1;
      termin.miesiac = -1;
      fscanf(zamowienia, "%d ", &termin.dzien);
      fscanf(zamowienia, "%d %d ", &termin.miesiac, &termin.rok);
      if((termin.rok != -1) && (termin.miesiac != -1)) {
         if((spis = realloc(spis, (i + 1) * sizeof(zamowienie))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         spis[i].termin = termin;
         fscanf(zamowienia, "%d ", &numer);
         spis[i].numer_zl = numer;
         spis[i].pozycje = NULL;
         fgets(nazwa, 80, zamowienia);
         if((spis[i].miasto = malloc((strlen(nazwa) + 1) * sizeof(char))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         strcpy(spis[i].miasto, nazwa);
         spis[i].numer_miasta = szukaj_indeksu_miasta(spis[i].miasto, miasta, liczba_miast);
         i++;
         j = 0;
      } else {
         fgets(nazwa, 80, zamowienia);
         if((spis[i - 1].pozycje = realloc(spis[i - 1].pozycje, (j + 1) * sizeof(pozycja))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         spis[i - 1].pozycje[j].ilosc = termin.dzien;
         if((spis[i - 1].pozycje[j].nazwa = malloc((strlen(nazwa) + 1) * sizeof(char))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         strcpy(spis[i - 1].pozycje[j].nazwa, nazwa);
         spis[i - 1].pozycje[j].ID = znajdz_ID(artykuly, liczba_artykulow, spis[i - 1].pozycje[j].nazwa);
         j++;
         spis[i - 1].ilosc_pozycji = j;
      }
   }
   free(spis[i - 1].pozycje[j - 1].nazwa);
   if((spis[i - 1].pozycje = realloc(spis[i - 1].pozycje, (j - 1) * sizeof(pozycja))) == NULL) {
      printf("Blad alokacji pamieci");
      exit(1);
   };
   spis[i - 1].ilosc_pozycji = j - 1;

   *liczba_zamowien = i;
   fclose(zamowienia);

   return spis;
}
