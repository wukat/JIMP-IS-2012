#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void leap_year(int year, int months_length[]) { //sprawdzenie czy przestepny
   if(((year + 1900) % 4 == 0) && ((year + 1900) % 4 != 0) || ((year + 1900) % 400 == 0)) { //jesli przestepny
      months_length[1] = 29; //to dlugosc lutego 29
   }
}

int set_first_day(int month, int current_month, int current_month_first_day, int months_length[]) { //ustalanie pierwszego dnia months_namesa
   int sum_of_days = 0;
   int first_day, i;

   if((month - 1) > current_month) { //dla miesiecy po obecny,
      for(i = current_month; i < month - 1; i++) {
         sum_of_days += months_length[i]; //suma dni (od teraz do zadanego miesiaca)
      }
      first_day = (sum_of_days + current_month_first_day + 1) % 7;
   } else if(month - 1 < current_month) { //dla miesiecy przed obecnym
      for(i = month - 1; i < current_month; i++) {
         sum_of_days += months_length[i]; //suma dni od zadanego miesiaca do teraz
      }
      first_day = 7 - (sum_of_days - current_month_first_day - 1) % 7;
   } else
      first_day = current_month_first_day;

   return first_day;
}

void print_calendar(FILE *fp, int month, int first_day, int months_length[], char months_names[12][12]) { //wyswietlanie
   int day, i, j;

   //drukujemy nazwe aktualnego
   fprintf(fp, "\\documentclass{article} \n\\usepackage[cp1250]{inputenc} \n\\begin{document} \n");
   fprintf(fp, "\\begin{table} \n\\begin{center} \n\\Large{\\textbf{%s}}", months_names[month - 1]);

   for(day = 1, i = 1, fprintf(fp,
         "\\\\\n\\begin{tabular}{|ccccccc|} \n\\hline \\textbf{PN} & \\textbf{WT} & \\textbf{ŒR} & \\textbf{CZW} & \\textbf{PT} & \\textbf{SB} & \\textbf{ND} \\\\\\hline");
         day <= months_length[month - 1]; i++) { //naglowek tabeli, poczatêk texu
      fprintf(fp, "\n");
      for(j = 1; j <= 7; j++) { //liczymy do 7
         if(i == 1) { //pierwszy tydzien
            if(j >= first_day) { //to zaczynamy pierwszego dnia miesiaca
               if(j == 7)
                  fprintf(fp, "%d \\\\", day++); //jesli to 7 dzien tygodnia, to inaczej konczymy linijke
               else
                  fprintf(fp, "%d & ", day++);
            } else {
               if(j != 7)
                  fprintf(fp, " & "); //biale znaki zanim dojdziemy do pierwszego dnia miesiaca
               else
                  fprintf(fp, " \\\\"); //albo konczymy linie jesli to 7 dzien
            }
         } else {
            if(day <= months_length[month - 1]) { //jesli nie doszlismy do konca miesiaca
               if(j == 7)
                  fprintf(fp, "%d \\\\", day++);
               else
                  fprintf(fp, "%d & ", day++);
            } else {
               if(j != 7)
                  fprintf(fp, " & "); //jesli miesiac sie skonczyl to dopisujemy puste pola tabeli
               else
                  fprintf(fp, " \\\\");
            }
         }
      }
   }
   fprintf(fp, "\\hline"); //obramowanie
   fprintf(fp, "\n\\end{tabular} \n\\end{center} \n\\end{table} \n\\end{document}"); //konczymy texa
}

int main() {
   int month, first_day;
   FILE *fp; //plik
   time_t wsk_sekund;
   time(&wsk_sekund);
   int current_month_first_day = (localtime(&wsk_sekund)->tm_mday - localtime(&wsk_sekund)->tm_wday) % 7; //obliczanie pierwszego dnia aktaulnego months_namesa

   //tablice do miesiecy
   char months_names[12][12] = { "STYCZEN", "LUTY", "MARZEC", "KWIECIEN", "MAJ", "CZERWIEC", "LIPIEC", "SIERPIEN",
         "WRZESIEN", "PAZDZIERNIK", "LISTOPAD", "GRUDZIEN" }; //tablica z nazwami miesiecy
   int months_length[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; //tablica dlugosci miesiecy

   //wczytanie numeru months_namesa
   do {
      printf("Podaj numer miesiaca: ");
      scanf("%2d", &month);
   } while((month < 1) || (month > 12));

   fp = fopen("kalendarz.tex", "w"); //tworzymy i otwieramy go
   leap_year(localtime(&wsk_sekund)->tm_year + 1900, months_length);
   first_day = set_first_day(month, localtime(&wsk_sekund)->tm_mon, current_month_first_day, months_length);
   print_calendar(fp, month, first_day, months_length, months_names);
   fclose(fp); //zamykamy plik
   printf("Zostal utworzony plik 'kalendarz.tex'.");

   return 0;
}
