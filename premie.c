#include <stdio.h>
#include <time.h>
#define size 4

void det_length_and_indexes(int * days, int * months, int * years, int * index, int * dlugosc);
void sort(int * index, int * dlugosc);
void get_bonus(int * index, int * length, double * amount, int length1, double bonus1, int length2, double bonus2,
      int length3, double bonus3);
void show_account(int * index, char ** surnames, double * amount);

int main(void) {
   char * surnames[] = { "Kowalski", "Nowak", "Jarosz", "Koperek" };
   double amount[] = { 1000, 500, 100, 10000 };
   int days[] = { 3, 4, 30, 5 };
   int months[] = { 5, 10, 7, 1 };
   int years[] = { 2000, 2006, 2001, 2012 };
   int indexes[size];
   int length[size];

   det_length_and_indexes(days, months, years, indexes, length);
   sort(indexes, length);

   get_bonus(indexes, length, amount, 360, 0.05, 90, 0.03, 30, 0.01);
   show_account(indexes, surnames, amount);

   return 0;
}

void det_length_and_indexes(int * days, int * months, int * years, int * index, int * length) {
   int i;
   time_t wsk_sekund;
   time(&wsk_sekund);

   for(i = 0; i < size; i++) {
      *(length + i) = (localtime(&wsk_sekund)->tm_year + 1900 - *(years + i)) * 360
            + (localtime(&wsk_sekund)->tm_mon - *(months + i)) * 30 + localtime(&wsk_sekund)->tm_mday - *(days + i);
      *(index + i) = i;
   }
}

void sort(int * index, int * length) {
   int j, i = 0;
   int temp, change = 1;

   while(change) {
      j = 0;
      change = 0;
      while(j < size - i - 1) {
         if(*(length + j) < *(length + j + 1)) {
            temp = *(index + j);
            *(index + j) = *(index + j + 1);
            *(index + j + 1) = temp;
            change = 1;
         }
         j++;
      }
      i++;
   }
}

void get_bonus(int * index, int * length, double * amount, int length1, double bonus1, int length2, double bonus2,
      int length3, double bonus3) {
   int i = 0;

   while(i < size) {
      if(*(length + *(index + i)) >= length1) {
         *(amount + *(index + i)) *= (1 + bonus1);
      } else {
         length1 = length2;
         length2 = length3;
         if(*(length + *(index + i)) >= length1) {
            *(amount + *(index + i)) *= (1 + bonus1);
         }
      }
      i++;
   }
}

void show_account(int * index, char ** surnames, double * amount) {
   int i = 0;

   while(i < size) {
      printf("%s: %f\n", *(surnames + *(index + i)), *(amount + *(index + i)));
      i++;
   }
}
