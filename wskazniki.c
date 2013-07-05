#include <stdio.h>
#include <stdlib.h>
/*W segmencie g��wnym nale�y zdefiniowa� tablic� jednowymiarow�, przypisa� jej elementom losowe warto�ci z zakresu [-100, 100],
 wyprowadzi� warto�ci element�w tablicy na ekran oraz wywo�a� funkcj�, kt�ra oblicza ile jest element�w >0 oraz do ka�dego 
 elementu <0 dodaje 1 (nale�y u�y� operatora ++). Nast�pnie (tak�e w main) wyprowadzi� na ekran liczb� element�w dodatnich
 oraz zawarto�� tablicy. W wersji rozszerzonej � to samo dla tablicy dwu- i trzywymiarowej.*/
#define rozm 3

//dla tablicy dwuwywmiarowej
int zlicz(int tablica[][rozm]);

int main(void) {
   int tablica[rozm][rozm], *wsk, i;

   i = 0;
   //losujemy
   while(i < rozm) {
      wsk = &tablica[i][0];
      while(wsk < &tablica[i][0] + rozm) {
         *wsk = (rand() % 201) - 100;
         printf("%d\n", *wsk);
         wsk++;
      }
      i++;
   }

   //wyswietlamy ile dodatnich
   printf("Liczba dodatnich: %d\n", zlicz(tablica));

   //wyswietlamy tablice
   i = 0;
   while(i < rozm) {
      wsk = &tablica[i][0];
      while(wsk < &tablica[i][0] + rozm) {
         printf("%d\n", *wsk);
         wsk++;
      }
      i++;
   }

   return 0;
}

//funkcja liczaca dodatnie i dodajaca 1 do ujemnych
int zlicz(int tablica[][rozm]) {
   int i, licz = 0;
   int * wsk = &tablica[0][0];

   i = 0;
   while(i < rozm) {
      while(wsk < &tablica[i][0] + rozm) {
         if(*wsk < 0)
            (*wsk)++;
         else if(*wsk > 0)
            licz++;
         wsk++;
      }
      i++;
   }

   return licz;
}
