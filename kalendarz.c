#include <stdio.h>
#include <time.h>

void leap_year(int year, int months_length[]) { //sprawdzenie czy przestepny
   if(((year + 1900) % 4 == 0) && ((year + 1900) % 4 != 0) || ((year + 1900) % 400 == 0)) { //i przestepny
      months_length[1] = 29; //to dlugosc lutego 29
   }
}

int set_first_day(int month, int current_month, int current_month_first_day, int months_length[]) { //ustalanie pierwszego dnia miesiaca
   int sum_of_days = 0;
   int first_day, i;

   if((month - 1) > current_month) {
      for(i = current_month; i < month - 1; i++) {
         sum_of_days += months_length[i];
      }
      first_day = (sum_of_days + current_month_first_day + 1) % 7;
   } else if(month - 1 < current_month) {
      for(i = month - 1; i < current_month; i++) {
         sum_of_days += months_length[i];
      }
      first_day = 7 - (sum_of_days - current_month_first_day - 1) % 7;
   } else
      first_day = current_month_first_day;

   return first_day;
}

void print_calendar(int month, int first_day, int months_length[], char months_names[12][12]) { //wyswietlanie
   int day, i, j;

   printf("%35s \n", months_names[month - 1]); //i drukujemy nazwe dla aktalnego

   for(day = 1, i = 1, printf("   PN   WT   SR   CZW  PT   SB   ND"); day <= months_length[month - 1]; i++) { //naglowek
      printf("\n");
      for(j = 1; j <= 7; j++) { //liczymy do 7
         if(i == 1) { //jesli po raz pierwszy
            if(j >= first_day) { //to zaczynamy od jakiegos dnia tygodnia (pierwszego miesiaca)
               printf("%5d", day++);
            } else {
               printf("     "); //czyli jesli licznik jeszcze nie doszedl do pierwszego dnia
            }
         } else {
            if(day <= months_length[month - 1]) { //jesli nie doszlismy do konca miesiaca
               printf("%5d", day++); //to drukujemy numer
            }
         }
      }
   }
}

int main() {
   int month, first_day;

   time_t wsk_sekund;
   time(&wsk_sekund);
   int current_month_first_day = (localtime(&wsk_sekund)->tm_mday - localtime(&wsk_sekund)->tm_wday) % 7;

   //ustalenie tablic z miesiacami
   char months_names[12][12] = { "styczen", "luty", "marzec", "kwiecien", "maj", "czerwiec", "lipiec", "sierpien",
         "wrzesien", "pazdziernik", "listopad", "grudzien" }; //tablica z nazwami miesiecy
   int months_length[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //tablica dlugosci miesiecy

   //wczytanie numeru miesiaca
   do {
      printf("Podaj numer miesiaca: ");
      scanf("%2d", &month);
   } while((month < 1) || (month > 12));

   leap_year(localtime(&wsk_sekund)->tm_year + 1900, months_length);
   first_day = set_first_day(month, localtime(&wsk_sekund)->tm_mon, current_month_first_day, months_length);
   print_calendar(month, first_day, months_length, months_names);

   return 0;
}
