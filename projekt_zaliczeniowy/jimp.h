#ifndef JIMP_H
#define JIMP_H
#include "struktury.h"

int szukaj_indeksu_miasta(char *, char **, int);
char ** lista_miast(int *, char *);
int ** macierz_odleglosci(int, char **, char *);
artykul * lista_artykulow(int *, char *);
long int znajdz_ID(artykul *, int, char *);
hurtownia * lista_hurtowni(artykul *, int, int *, char **, int, char *);
zamowienie * lista_zamowien(char **, int, artykul *, int, int *, char *);
hurtownia * usun_przeterminowane(hurtownia *, int, data);
hurtownia * posortuj_po_datach(hurtownia *, int);
void stworz_liste_przeterminowanych(int, hurtownia *, int, data, char *);
int ** dijkstra(int **, int, int);
int ** przepisz(int **, int, int);
int * tworz_liste_sasiadow(int **, int, int);
int indeks_minimum_tablicy(int **, int);
int roznica_dat(data, data);
data dodaj_do_daty(data, int);
hurtownia * realizuj_zamowienia(data *, char *, int *);
void realizuj_to_zlecenie(zamowienie, int **, int, hurtownia *, int, FILE *);
int ** sortuj(int **, int);
FILE * otworz_plik(char *, char *, char *);
hurtownia * lista_przydatnych_hurtowni(zamowienie, hurtownia *, int, int *);
char * znajdz_nazwe(artykul *, int, long int);
int znajdz_odleglosc(int **, int, int);
void wpisz_hurtownie_do_pliku(hurtownia *, int, char *);
void znajdz_koncowki(int, data, char *, hurtownia *, int);
void szukaj_wyczerpanych(zamowienie, int **, int, hurtownia *, int, FILE *, data);
sklad * sumuj_hurtownie(hurtownia *, int, int *);


#endif
