#include <stdio.h>

void add_matrix(int (*a)[3], int (*b)[3], int (*c)[3], int size);
void show_matrix(int (*c)[3], int size);
void multiply_matrix(int (*a)[3], int (*b)[3], int (*c)[3], int size);

int main(void) {
   int A[][3] = { { 1, 3, 5 }, { 2, 7, 10 }, { 9, 11, 0 } };
   int B[][3] = { { 5, 9, 0 }, { 5, 31, 2 }, { 4, 9, 9 } };
   int i, j, c[3][3];

   add_matrix(A, B, c, 3);
   printf("Dodane macierze: \n");
   show_matrix(c, 3);
   for(i = 0; i < 3; i++)
      for(j = 0; j < 3; j++)
         (*(c + i))[j] = 0;
   multiply_matrix(A, B, c, 3);
   printf("Pomnozone macierze: \n");
   show_matrix(c, 3);

   return 0;
}

void add_matrix(int (*a)[3], int (*b)[3], int (*c)[3], int size) {
   int i, j;

   for(i = 0; i < size; i++)
      for(j = 0; j < 3; j++) {
         (*(c + i))[j] = (*(a + i))[j] + (*(b + i))[j];
      }
}

void show_matrix(int (*c)[3], int size) {
   int i, j;

   for(i = 0; i < size; i++) {
      for(j = 0; j < 3; j++) {
         printf("%d ", (*(c + i))[j]);
      }
      printf("\n");
   }
}

void multiply_matrix(int (*a)[3], int (*b)[3], int (*c)[3], int size) {
   int i, j, k;

   for(i = 0; i < size; i++) {
      for(j = 0; j < 3; j++) {
         for(k = 0; k < 3; k++) {
            (*(c + i))[j] += (*(a + i))[k] * (*(b + k))[j];
         }
      }
   }
}
