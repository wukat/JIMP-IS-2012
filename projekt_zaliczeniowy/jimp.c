#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jimp.h"
#define n 30
#define cenakm 0.5
#define n1 20

int main(int argc, char **argv) {
   hurtownia * hurtownie;
   int liczba_hurtowni;
   data dzis = { 1, 12, 2012 };

   if(argc == 1) {
      printf("Nie podales katalogu!");
      exit(1);
   }

   hurtownie = realizuj_zamowienia(&dzis, argv[1], &liczba_hurtowni);
   stworz_liste_przeterminowanych(n, hurtownie, liczba_hurtowni, dzis, argv[1]);
   wpisz_hurtownie_do_pliku(hurtownie, liczba_hurtowni, argv[1]);
   znajdz_koncowki(n1, dzis, argv[1], hurtownie, liczba_hurtowni);

   return 0;
}

hurtownia * usun_przeterminowane(hurtownia * hurtownie, int ilosc_hurtowni, data dzis) {
   int i, j;

   for(i = 0; i < ilosc_hurtowni; i++) {
      j = 0;
      while((roznica_dat(dzis, hurtownie[i].produkty[j].waznosc) >= 0)
            && ((hurtownie[i].produkty[j].waznosc.dzien != 0) && (hurtownie[i].produkty[j].waznosc.miesiac != 0)
                  && (hurtownie[i].produkty[j].waznosc.rok != 0)) && (j < hurtownie[i].ilosc_pozycji)) {
         hurtownie[i].produkty[j].ilosc = 0;
         j++;
      }
   }

   return hurtownie;
}

void stworz_liste_przeterminowanych(int prog, hurtownia * hurtownie, int ilosc_hurtowni, data dzis, char * katalog) {
   FILE * przeterminowane;
   int i, j, k, wpisz, rozmiar_tymczasowej = 0;
   char ** tymczasowe = NULL;

   przeterminowane = otworz_plik(katalog, "przeterminowane.dat", "w");

   for(i = 0; i < ilosc_hurtowni; i++)
      for(j = 0; j < hurtownie[i].ilosc_pozycji; j++) {
         wpisz = 1;
         if((roznica_dat(hurtownie[i].produkty[j].waznosc, dzis) <= prog)
               && ((hurtownie[i].produkty[j].waznosc.dzien != 0) && (hurtownie[i].produkty[j].waznosc.miesiac != 0)
                     && (hurtownie[i].produkty[j].waznosc.rok != 0))) {
            for(k = 0; k < rozmiar_tymczasowej; k++)
               if((wpisz == 1) && (strcmp(hurtownie[i].produkty[j].nazwa, tymczasowe[k]) == 0))
                  wpisz = 0;
            if(wpisz == 1) {
               fprintf(przeterminowane, "%s", hurtownie[i].produkty[j].nazwa);
               rozmiar_tymczasowej++;
               if((tymczasowe = realloc(tymczasowe, rozmiar_tymczasowej * sizeof(char *))) == NULL) {
                  printf("Blad alokacji pamieci");
                  exit(1);
               }
               if((tymczasowe[k] = malloc((strlen(hurtownie[i].produkty[j].nazwa) + 1) * sizeof(char))) == NULL) {
                  printf("Blad alokacji pamieci");
                  exit(1);
               }
               strcpy(tymczasowe[k], hurtownie[i].produkty[j].nazwa);
            }
         }
      }

   for(i = 0; i < rozmiar_tymczasowej; i++)
      free(tymczasowe[i]);
   free(tymczasowe);

   fclose(przeterminowane);
}

hurtownia * realizuj_zamowienia(data * dzis, char * katalog, int * liczba_hurtowni) {
   int i, j;
   int liczba_miast, liczba_zamowien, liczba_artykulow;
   char ** miasta = NULL;
   int ** odleglosci = NULL;
   hurtownia * hurtownie;
   zamowienie * zlecenia;
   artykul * artykuly;
   FILE * realizacja;

   realizacja = otworz_plik(katalog, "realizacja_zlecen.dat", "w");

   artykuly = lista_artykulow(&liczba_artykulow, katalog);
   miasta = lista_miast(&liczba_miast, katalog);
   zlecenia = lista_zamowien(miasta, liczba_miast, artykuly, liczba_artykulow, &liczba_zamowien, katalog);
   odleglosci = macierz_odleglosci(liczba_miast, miasta, katalog);
   hurtownie = lista_hurtowni(artykuly, liczba_artykulow, liczba_hurtowni, miasta, liczba_miast, katalog);

   for(i = 0; i < liczba_artykulow; i++)
      free(artykuly[i].nazwa);
   free(artykuly);

   for(i = 0; i < liczba_zamowien; i++) {
      *dzis = zlecenia[i].termin;
      hurtownie = usun_przeterminowane(hurtownie, *liczba_hurtowni, *dzis);
      zlecenia[i].numer_miasta = szukaj_indeksu_miasta(zlecenia[i].miasto, miasta, liczba_miast);
      realizuj_to_zlecenie(zlecenia[i], odleglosci, liczba_miast, hurtownie, *liczba_hurtowni, realizacja);
   }

   /*zwalniamy pamiec*/
   for(i = 0; i < liczba_miast; i++)
      free(miasta[i]);
   free(miasta);

   for(i = 0; i < liczba_miast; i++)
      free(odleglosci[i]);
   free(odleglosci);

   for(i = 0; i < liczba_zamowien; i++) {
      for(j = 0; j < zlecenia[i].ilosc_pozycji; j++)
         free(zlecenia[i].pozycje[j].nazwa);
      free(zlecenia[i].pozycje);
      free(zlecenia[i].miasto);
   }
   free(zlecenia);

   return hurtownie;
}

void realizuj_to_zlecenie(zamowienie zlecenie, int ** odleglosci, int l_miast, hurtownia * hurtownie, int l_hurtowni,
      FILE * realizacja) {
   int i, j, k, l, check, aktualne_miejsce, liczba_przydatnych_hurtowni, wykonane = 0;
   int ** najkrotsze = NULL;
   hurtownia * przydatne_hurtownie = NULL;
   double cena = 0.0;

   aktualne_miejsce = zlecenie.numer_miasta;
   najkrotsze = dijkstra(odleglosci, l_miast, aktualne_miejsce);
   przydatne_hurtownie = lista_przydatnych_hurtowni(zlecenie, hurtownie, l_hurtowni, &liczba_przydatnych_hurtowni);

   fprintf(realizacja, "%d %d %d %d %s", zlecenie.termin.dzien, zlecenie.termin.miesiac, zlecenie.termin.rok,
         zlecenie.numer_zl, zlecenie.miasto);
   while((liczba_przydatnych_hurtowni > 0) && (wykonane == 0)) {
      aktualne_miejsce = -1;
      for(j = 0; j < l_miast; j++)
         for(i = 0; i < liczba_przydatnych_hurtowni; i++) {
            if((aktualne_miejsce == -1) && (najkrotsze[j][2] == przydatne_hurtownie[i].numer_miasta)) {
               aktualne_miejsce = przydatne_hurtownie[i].numer_miasta;
               cena += cenakm * najkrotsze[j][1];
               check = 0;
               for(j = 0; j < przydatne_hurtownie[i].ilosc_pozycji; j++) {
                  if(przydatne_hurtownie[i].produkty[j].ilosc > 0) {
                     for(l = 0; l < zlecenie.ilosc_pozycji; l++) {
                        if(zlecenie.pozycje[l].ilosc > 0) {
                           if(zlecenie.pozycje[l].ID == przydatne_hurtownie[i].produkty[j].ID) {
                              if(check == 0) {
                                 fputs(przydatne_hurtownie[i].miasto, realizacja);
                                 check = 1;
                              }
                              if(zlecenie.pozycje[l].ilosc >= przydatne_hurtownie[i].produkty[j].ilosc) {
                                 fprintf(realizacja, "  %d %.2f %s", przydatne_hurtownie[i].produkty[j].ilosc,
                                       przydatne_hurtownie[i].produkty[j].cena,
                                       przydatne_hurtownie[i].produkty[j].nazwa);
                                 zlecenie.pozycje[l].ilosc -= przydatne_hurtownie[i].produkty[j].ilosc;
                                 cena += przydatne_hurtownie[i].produkty[j].ilosc
                                       * przydatne_hurtownie[i].produkty[j].cena;
                                 przydatne_hurtownie[i].produkty[j].ilosc = 0;
                              } else {
                                 fprintf(realizacja, "  %d %.2f %s", zlecenie.pozycje[l].ilosc,
                                       przydatne_hurtownie[i].produkty[j].cena,
                                       przydatne_hurtownie[i].produkty[j].nazwa);
                                 przydatne_hurtownie[i].produkty[j].ilosc -= zlecenie.pozycje[l].ilosc;
                                 cena += zlecenie.pozycje[l].ilosc * przydatne_hurtownie[i].produkty[j].cena;
                                 zlecenie.pozycje[l].ilosc = 0;
                              }
                           }
                        }
                     }
                  }
               }
            }
         }

      for(i = 0; i < l_miast; i++)
         free(najkrotsze[i]);
      free(najkrotsze);
      najkrotsze = dijkstra(odleglosci, l_miast, aktualne_miejsce);
      free(przydatne_hurtownie);
      przydatne_hurtownie = NULL;
      przydatne_hurtownie = lista_przydatnych_hurtowni(zlecenie, hurtownie, l_hurtowni, &liczba_przydatnych_hurtowni);

      wykonane = 1;
      for(i = 0; i < zlecenie.ilosc_pozycji; i++)
         if(zlecenie.pozycje[i].ilosc != 0)
            wykonane = 0;
   }
   cena += cenakm * znajdz_odleglosc(najkrotsze, l_miast, zlecenie.numer_miasta);
   if(wykonane == 0)
      fprintf(realizacja, "Zlecenie nie zostalo w pelni zrealizowane. Koszt czesciowej realizacji: %.2f.\n\n", cena);
   else
      fprintf(realizacja, "Zlecenie zostalo w pelni zrealizowane. Koszt realizacji: %.2f.\n\n", cena);

   for(i = 0; i < l_miast; i++)
      free(najkrotsze[i]);
   free(najkrotsze);
   free(przydatne_hurtownie);
}

void wpisz_hurtownie_do_pliku(hurtownia * hurtownie, int liczba_hurtowni, char * katalog) {
   FILE * akt_hurtownie;
   int i, j;

   akt_hurtownie = otworz_plik(katalog, "hurtownia_stan1.dat", "w");

   for(i = 0; i < liczba_hurtowni; i++) {
      fputs(hurtownie[i].miasto, akt_hurtownie);
      for(j = 0; j < hurtownie[i].ilosc_pozycji; j++)
         fprintf(akt_hurtownie, "%ld %d %.2f %d %d %d\n", hurtownie[i].produkty[j].ID, hurtownie[i].produkty[j].ilosc,
               hurtownie[i].produkty[j].cena, hurtownie[i].produkty[j].waznosc.dzien,
               hurtownie[i].produkty[j].waznosc.miesiac, hurtownie[i].produkty[j].waznosc.rok);
   }
   fputc('\n', akt_hurtownie);

   fclose(akt_hurtownie);
}

void znajdz_koncowki(int prog, data dzis, char * katalog, hurtownia * hurtownie, int liczba_hurtowni) {
   int i, j;
   int liczba_miast, liczba_zamowien, liczba_artykulow;
   char ** miasta = NULL;
   int ** odleglosci = NULL;
   zamowienie * zlecenia;
   artykul * artykuly;
   data aktualna, poczatkowa;
   FILE * koncowki;

   koncowki = otworz_plik(katalog, "koncowki.dat", "w");

   artykuly = lista_artykulow(&liczba_artykulow, katalog);
   miasta = lista_miast(&liczba_miast, katalog);
   odleglosci = macierz_odleglosci(liczba_miast, miasta, katalog);

   do {
      zlecenia = lista_zamowien(miasta, liczba_miast, artykuly, liczba_artykulow, &liczba_zamowien, katalog);

      poczatkowa = zlecenia[0].termin;
      for(i = 0; i < liczba_zamowien; i++) {
         aktualna = zlecenia[i].termin;
         if(roznica_dat(aktualna, poczatkowa) < prog) {
            if(i == 0)
               dzis = dodaj_do_daty(dzis, 1);
            else
               dzis = dodaj_do_daty(dzis, roznica_dat(aktualna, zlecenia[i - 1].termin));
            szukaj_wyczerpanych(zlecenia[i], odleglosci, liczba_miast, hurtownie, liczba_hurtowni, koncowki, dzis);
         }
      }
   } while((prog = prog - roznica_dat(zlecenia[liczba_zamowien - 1].termin, zlecenia[0].termin) - 1) > 0);

   for(i = 0; i < liczba_artykulow; i++)
      free(artykuly[i].nazwa);
   free(artykuly);

   for(i = 0; i < liczba_miast; i++)
      free(miasta[i]);
   free(miasta);

   for(i = 0; i < liczba_miast; i++)
      free(odleglosci[i]);
   free(odleglosci);

   for(i = 0; i < liczba_zamowien; i++) {
      for(j = 0; j < zlecenia[i].ilosc_pozycji; j++)
         free(zlecenia[i].pozycje[j].nazwa);
      free(zlecenia[i].pozycje);
      free(zlecenia[i].miasto);
   }
   free(zlecenia);

   for(i = 0; i < liczba_hurtowni; i++) {
      for(j = 0; j < hurtownie[i].ilosc_pozycji; j++)
         free(hurtownie[i].produkty[j].nazwa);
      free(hurtownie[i].produkty);
      free(hurtownie[i].miasto);
   }
   free(hurtownie);

   fclose(koncowki);
}

void szukaj_wyczerpanych(zamowienie zlecenie, int ** odleglosci, int l_miast, hurtownia * hurtownie, int l_hurtowni,
      FILE * koncowki, data dzis) {
   int i, j, k, l, liczba_przydatnych_hurtowni, aktualne_miejsce;
   int ** najkrotsze = NULL;
   hurtownia * przydatne_hurtownie = NULL;
   int * ilosci, dlugosc;
   sklad * stan_hurtowni1, *stan_hurtowni2;

   aktualne_miejsce = zlecenie.numer_miasta;
   stan_hurtowni1 = sumuj_hurtownie(hurtownie, l_hurtowni, &dlugosc);
   hurtownie = usun_przeterminowane(hurtownie, l_hurtowni, dzis);
   najkrotsze = dijkstra(odleglosci, l_miast, aktualne_miejsce);

   przydatne_hurtownie = lista_przydatnych_hurtowni(zlecenie, hurtownie, l_hurtowni, &liczba_przydatnych_hurtowni);

   while(liczba_przydatnych_hurtowni > 0) {
      aktualne_miejsce = -1;
      for(j = 0; j < l_miast; j++)
         for(i = 0; i < liczba_przydatnych_hurtowni; i++) {
            if((aktualne_miejsce == -1) && (najkrotsze[j][2] == przydatne_hurtownie[i].numer_miasta)) {
               aktualne_miejsce = przydatne_hurtownie[i].numer_miasta;
               for(j = 0; j < przydatne_hurtownie[i].ilosc_pozycji; j++) {
                  if(przydatne_hurtownie[i].produkty[j].ilosc > 0) {
                     for(l = 0; l < zlecenie.ilosc_pozycji; l++) {
                        if(zlecenie.pozycje[l].ilosc > 0) {
                           if(zlecenie.pozycje[l].ID == przydatne_hurtownie[i].produkty[j].ID) {
                              if(zlecenie.pozycje[l].ilosc >= przydatne_hurtownie[i].produkty[j].ilosc) {
                                 zlecenie.pozycje[l].ilosc -= przydatne_hurtownie[i].produkty[j].ilosc;
                                 przydatne_hurtownie[i].produkty[j].ilosc = 0;
                              } else {
                                 przydatne_hurtownie[i].produkty[j].ilosc -= zlecenie.pozycje[l].ilosc;
                                 zlecenie.pozycje[l].ilosc = 0;
                              }
                           }
                        }
                     }
                  }
               }
            }
         }

      for(i = 0; i < l_miast; i++)
         free(najkrotsze[i]);
      free(najkrotsze);
      najkrotsze = dijkstra(odleglosci, l_miast, aktualne_miejsce);
      free(przydatne_hurtownie);
      przydatne_hurtownie = NULL;
      przydatne_hurtownie = lista_przydatnych_hurtowni(zlecenie, hurtownie, l_hurtowni, &liczba_przydatnych_hurtowni);\
   }

   stan_hurtowni2 = sumuj_hurtownie(hurtownie, l_hurtowni, &dlugosc);

   for(i = 0; i < dlugosc; i++) {
      if((stan_hurtowni1[i].ilosc != 0) && (stan_hurtowni2[i].ilosc == 0))
         fprintf(koncowki, "%s", stan_hurtowni1[i].nazwa);
   }
   free(stan_hurtowni1);
   free(stan_hurtowni2);
}

sklad * sumuj_hurtownie(hurtownia * hurtownie, int liczba_hurtowni, int * dlugosc) {
   int i, j, k, znajdz;
   sklad * spis = NULL;
   *dlugosc = 0;

   for(i = 0; i < liczba_hurtowni; i++) {
      for(j = 0; j < hurtownie[i].ilosc_pozycji; j++) {
         znajdz = 0;
         for(k = 0; k < *dlugosc; k++)
            if(hurtownie[i].produkty[j].ID == spis[k].ID) {
               spis[k].ilosc += hurtownie[i].produkty[j].ilosc;
               znajdz = 1;
            }
         if(znajdz == 0) {
            (*dlugosc)++;
            if((spis = realloc(spis, *dlugosc * sizeof(sklad))) == NULL) {
               printf("Blad alokacji pamieci!");
               exit(1);
            }
            if((spis[*dlugosc - 1].nazwa = malloc((strlen(hurtownie[i].produkty[j].nazwa) + 1) * sizeof(char)))
                  == NULL) {
               printf("Blad alokacji pamieci!");
               exit(1);
            }
            strcpy(spis[*dlugosc - 1].nazwa, hurtownie[i].produkty[j].nazwa);
            spis[*dlugosc - 1].ilosc = hurtownie[i].produkty[j].ilosc;
            spis[*dlugosc - 1].ID = hurtownie[i].produkty[j].ID;
         }
      }
   }

   return spis;
}

hurtownia * lista_przydatnych_hurtowni(zamowienie zlecenie, hurtownia * hurtownie, int liczba_hurtowni,
      int * liczba_przydatnych_hurtowni) {
   /*dodajemy te hurtownie, ktore maja produkty potrzebne do zamowienia*/
   hurtownia * spis = NULL;
   int i, j, k, licznik, znalezione;

   licznik = 0;
   for(i = 0; i < liczba_hurtowni; i++) {
      znalezione = 0;
      for(j = 0; j < hurtownie[i].ilosc_pozycji; j++) {
         if(znalezione == 0) {
            for(k = 0; k < zlecenie.ilosc_pozycji; k++) {
               if((zlecenie.pozycje[k].ilosc != 0) && (hurtownie[i].produkty[j].ilosc != 0)
                     && (zlecenie.pozycje[k].ID == hurtownie[i].produkty[j].ID)) {
                  znalezione = 1;
                  licznik++;
               }
            }
         }
      }
      if(znalezione == 1) {
         if((spis = realloc(spis, (licznik) * sizeof(hurtownia))) == NULL) {
            printf("Blad alokacji pamieci");
            exit(1);
         }
         spis[licznik - 1] = hurtownie[i];
      }
   }
   *liczba_przydatnych_hurtowni = licznik;

   return spis;
}

