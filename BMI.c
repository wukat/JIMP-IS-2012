#include <stdio.h>
#include <stdlib.h>

int main(void) {
   int weight;
   float height;
   float BMI;

   printf("Content-Type: text/html; charset=utf-8\n\n");
   printf("<title>BMI</title>");
   printf(
         "<form action=\"./BMI.cgi\"method=\"POST\">\
			<input type=\"text\" name = \"waga (w kg)\">\
			<input type=\"text\" name = \"wzrost (w cm)\">\
			<input type=\"submit\">\ ");

   if(getenv("CONTENT_LENGTH") != 0) {
      scanf("%d", &weight);
      scanf("%.0f", &height);

      height /= 100;
      BMI = (float) weight / (height * height);

      printf("Twoj BMI wynosi %.2f.", BMI);
   }

   return 0;
}
