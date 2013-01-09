#include <stdio.h>
#include <stdlib.h>
  
struct okreg {
  char * nazwa;
  int numer;
  int glosy;
};
  
struct kandydat {
  char * nazwisko;
  char * imie;
  struct okreg * o;
  int glosy;
  int numer;		
};

void sortuj_po_okregach(int * indexes, struct kandydat * kandyd, int size);
void sortuj_glosy(int * indexes, struct kandydat * kandyd, int size);
void licz_glosy(struct kandydat * kandyd, int size);
void wyswietl_wyniki(int * indexes, struct kandydat * kandyd, struct okreg * okr, int size);

int main(void)
{  
  struct okreg okregi[] = {
    {"pierwszy", 1, 0},
    {"drugi", 2, 0},
    {"trzeci", 3, 0},
    {"czwarty", 4, 0},
    {"piaty", 5, 0}
  };
  
  struct kandydat kandydaci[] = {
    {"Kowalski", "Jan", NULL, 35412, 1},
    {"Nowak", "Wojciech", NULL, 53, 1},
    {"Hopek", "Konrad", NULL, 12, 3},
    {"Kononowicz", "Marian", NULL, 542354, 4},
    {"Kamilak", "Ola", NULL, 312, 5},
    {"Baran", "Wiktor", NULL, 12354, 3},
    {"ja","ja", NULL, 11, 5}
  };
  int *indexes, i; 
  int size = sizeof(kandydaci)/sizeof(struct kandydat); 
 
  kandydaci[0].o = okregi;
  kandydaci[1].o = okregi + 4;
  kandydaci[2].o = okregi;
  kandydaci[3].o = okregi + 1;
  kandydaci[4].o = okregi + 4;
  kandydaci[5].o = okregi + 1;
  kandydaci[6].o = okregi + 1;
  
  indexes = malloc(size * sizeof(int));
  for (i = 0; i < size; i++)
    *(indexes + i) = i;

  sortuj_po_okregach(indexes, kandydaci, size);
  sortuj_glosy(indexes, kandydaci, size);
  licz_glosy(kandydaci, size);
  wyswietl_wyniki(indexes, kandydaci, okregi, size);
  

  free(indexes);
    
  return 0;
}


void sortuj_po_okregach(int * indexes, struct kandydat * kandyd, int size)
{
  int i, j, zamiana, temp;

  zamiana = 1;
  j = 1;
  
  while (zamiana)
  {
    zamiana = 0;
    i = 0;
    while (i < size - j)
    {
      if ((kandyd + *(indexes + i))->o > (kandyd + *(indexes + i + 1))->o)
      {
        temp = *(indexes + i);
        *(indexes + i) = *(indexes + i + 1);
        *(indexes + i + 1) = temp;
        zamiana = 1;
      }
      i++;
    }		
    j++;
  }
}


void sortuj_glosy(int * indexes, struct kandydat * kandyd, int size)
{
  int i, j, zamiana, temp;

  zamiana = 1;
  j = 1;
  
  while (zamiana)
  {
    zamiana = 0;
    i = 0;
    while (i < size - j)
    {
      if (((kandyd + *(indexes + i))->o == (kandyd + *(indexes + i + 1))->o) && ((kandyd + *(indexes + i))->glosy < (kandyd + *(indexes + i + 1))->glosy))
      {
        temp = *(indexes + i);
        *(indexes + i) = *(indexes + i + 1);
        *(indexes + i + 1) = temp;
        zamiana = 1;
      }
      i++;
    }		
    j++;
  }
}


void licz_glosy(struct kandydat * kandyd, int size)
{
  int i = 0;

  while (i < size)
  {
    ((kandyd + i)->o)->glosy += (kandyd + i)->glosy;
    i++;
  }  
}


void wyswietl_wyniki(int * indexes, struct kandydat * kandyd, struct okreg * okr, int size)
{
  int i = 0;
  int j = 0;

  while (i < size)
  {
    if ((i == 0) || ((kandyd + *(indexes + i))->o != (kandyd + *(indexes + i - 1))->o))
    {
      printf ("\n%d: %s ---- %d \n", (okr + j)->numer, (okr + j)->nazwa, (okr + j)->glosy);
      j++;
    }
    printf("  %d. %s %s ---- %d\n", (kandyd + *(indexes + i))->numer, (kandyd + *(indexes + i))->imie, 
(kandyd + *(indexes + i))->nazwisko, (kandyd + *(indexes + i))->glosy);
    i++;
  }
}
