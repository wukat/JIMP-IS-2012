#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//program deszyfrujacy
void decrypt_file(FILE * crypt, int, FILE * decrypted);
int find_shift(FILE * crypt);
char maximum(int[25]);

int main(void)
{
 	FILE * crypt, * decrypted; 
 	int shift;

	//otwieramy dwa pliki
	if((crypt = fopen("szyfr_v1.txt", "r")) == NULL)
  {
    perror("Blad otwarcia pliku");
    exit(1);
  }
  decrypted = fopen("deszyfr.txt", "w");
  
  shift = find_shift(crypt);  //ustalamy przesuniecie
  rewind(crypt);  //wracamy na poczatek pliku
	decrypt_file(crypt, ahift, decrypted);  //odszyfrowujemy
	
	printf("Udanie odszyfrowano i zapisano do pliku!");
	fclose(crypt);
	fclose(decrypted);
}


void decrypt_file(FILE * crypt, int shift, FILE * decrypted)  //odszufrowanie
{
	int ch;
	int i = 1;
	
	while ((ch = getc(crypt)) != EOF)
	{
		if ((ch >= 'a') && (ch <= 'z')) 
		{
			if ((ch - shift >= 'a') && (ch - shift <= 'z'))
			{ ch -= shift;}
			else 
			{ ch += 'z' - 'a' - shift + 1;}
		}
		else if ((ch >= 'A') && (ch <= 'Z'))
		{
			if ((ch - shift >= 'A') && (ch - shift <= 'Z'))
			{ ch -= shift;}
			else
			{ ch += 'Z' - 'A' - shift + 1;}
		}
		
		putc(ch, decrypted);
	}
}

int find_shift(FILE * crypt)  //szukanie przesuniecia
{
	int i, ch, shift;  
	int signs[25];
	int common[4];
	
	for (ch = 0; ch < 25; ch++)  //zerujemy tablice czestosci 
	  signs[ch] = 0;
	
	while ((ch = getc(crypt)) != EOF)  //zliczamy liczbe wystapien
	{
		if ((ch >= 'A') && (ch <= 'Z'))
		{ ch += ('a' - 'A');}
		if ((ch >= 'a') && (ch <= 'z'))  
		signs[ch - 'a']++;
	}
	
	for (i = 0; i < 4; i++)  //ustalamy 4 najczestsze
  {
		common[i] = maximum(signs);
		signs[common[i]] = 0;  //zerujemy znaleziorny, zeby wyszukac kolejny
  }
  
  i = 0; 
	shift = -1;
	
	do { 
    switch(common[i] - common[i + 1]) //dzialamy na roznicach
    {
      case 8: shift = common[i] + 'a' - 'i'; break;  //8 to i - a 
      case -8: shift = common[i]; break;  //a -i
      case 10: shift = common[i] + 'a' - 'o'; break;  //o - e
      case -10: shift = common[i] + 'a' - 'e'; break;  //e - o
      case 6: shift = common[i] + 'a' - 'o'; break;  //o - i
      case -6: shift = common[i] + 'a' - 'i'; break;  //i - 0
      case 14: shift = common[i] + 'a' - 'o'; break;  //o - a
      case -14: shift = common[i]; break;  //a - o
    }
    i++;  
  } while ((shift == -1) && (i < 3));
  
  if (shift < 0)
    shift = 26 + shift;  //robimy tak zeby przesuniecie bylo dodatnie
  
  return shift;
}

char maximum(int array[25])  //szukanie maksimum tablicy
{
	int i, index_max, max = 0;
	
	for (i = 0; i < 25; i++)
	  if (array[i] > max)
	  { 
			max = array[i];
			index_max = i;
		}
		
	return index_max;  //zwracanie indeksu maksymalnego elementu
}
