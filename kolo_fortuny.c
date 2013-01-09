#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void count_words(long int * alfabet_p, FILE * diction);
char * random_pass(FILE * diction, long int * alfabet_p);
char * random_word(FILE * diction, char letter, long int * alfabet_p);
char ** random_cross(FILE * diction, long int * alfabet_p, char * passwd);
void free_mem(int pass_leng, char ** cross);
int show(int pass_leng, char **cross, char *used_signes);
void get_sign(char *used_signs);
int count_points(char *used_signs);
void save_in_file(char * name, int points);


int main(void)
{
  FILE * diction;
  int pass_leng, end, i, points;
  char used[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, *used_signs = used;
  char ** cross;
  char * password, name[30];
  long int alfabet[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, *alfabet_p = alfabet;

  srand(time(0));
  if((diction = fopen("slownik.txt", "r")) == NULL)
  {
    perror("Blad otwarcia pliku");
    exit(1);
  }

  printf("Podaj swoje imie: \n");
  scanf("%s", name);
  getchar();
  count_words(alfabet_p, diction);
  password = random_pass(diction, alfabet_p);
  pass_leng = strlen(password);
  cross = random_cross(diction, alfabet_p, password);
  end = show(pass_leng, cross, used_signs);
  do {
    for (i = 0; i < 26; i++)
    {if (used[i] > 0) used[i] = 1;}
    get_sign(used_signs);
    end = show(pass_leng, cross, used_signs);
  } while (end);
  points = count_points(used_signs);
  if (points < 0) points = 0;
  printf("\nWygrales %s, zdobyles %d punktow! \n", name, points);
  save_in_file(name, points);
  free_mem(pass_leng, cross);
  fclose (diction);

  return 0;
}


void save_in_file(char * name, int points)
{
  FILE * results;
  int temp_points, i, done, j;
  char temp_name[40];
  int * points_array = NULL;
  char ** names_array = NULL;
  
  if((results = fopen("wyniki.txt", "r")) == NULL)
  {
    perror("Blad otwarcia pliku");
    exit(1);
  }

  done = 0;
  i = 0;
  while(!feof(results))
  {
    fscanf(results, "%d", &temp_points);
    fgetc(results);
    if (fscanf(results, "%s", temp_name) == 1) 
    {
      if ((done == 0) && (points > temp_points))
      {
        names_array = realloc(names_array, (i + 1) * sizeof(char *));
        points_array = realloc(points_array, (i + 1) * sizeof(int));
        names_array[i] = malloc((strlen(name) + 1) * sizeof(char));
        strcpy(names_array[i], name);
        points_array[i] = points;
        i++;
        done = 1;
      }
      names_array = realloc(names_array, (i + 1) * sizeof(char *));
      points_array = realloc(points_array, (i + 1) * sizeof(int));
      names_array[i] = malloc((strlen(temp_name) + 1) * sizeof(char));
      strcpy(names_array[i], temp_name);
      points_array[i] = temp_points;
      i++;
    }
  }
  if (done == 0)
  {
    names_array = realloc(names_array, (i + 1) * sizeof(char *));
    points_array = realloc(points_array, (i + 1) * sizeof(int));
    names_array[i] = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(names_array[i], name);
    points_array[i] = points;
    i++;
  }
  fclose(results);

  if((results = fopen("wyniki.txt", "w")) == NULL)
  {
    perror("Blad otwarcia pliku");
    exit(1);
  }
  
  for (j = 0; j < i; j++)
  {
    fprintf(results, "%d %s\n", points_array[j], names_array[j]);
    free(names_array[j]);
  }
  free(points_array);
  free(names_array);

  fclose(results);
}


void count_words(long int * alfabet_p, FILE * diction)
{
  char first_letter = 'a';
  char words[130];
	
  alfabet_p++;
  while(!feof(diction))
  {
    fgets(words, 130, diction);
    if (words[0] == first_letter) 
      { (*alfabet_p)++;	}
    else
    { 
      alfabet_p++;
      first_letter++;
      *alfabet_p = *(alfabet_p - 1);
      if (words[0] == first_letter) 
        (*alfabet_p)++;
    }
  }
}


int count_points(char *used_signs)
{
  int i, points = 0;
	
  for (i = 1; i < 4; i++)
    points += used_signs[i] * 10 - 10;
  for (i = 5; i < 8; i++)
    points += used_signs[i] * 10 - 10;
  for (i = 9; i < 14; i++)
    points += used_signs[i] * 10 - 10;
  for (i = 15; i < 20; i++)
    points += used_signs[i] * 10 - 10;
  for (i = 21; i < 24; i++)
    points += used_signs[i] * 10 - 10;
  points -= used_signs[0] * 10 - 10;
  points -= used_signs[4] * 10 - 10;
  points -= used_signs[8] * 10 - 10;
  points -= used_signs[14] * 10 - 10;
  points -= used_signs[20] * 10 - 10;
  points -= used_signs[24] * 10 - 10;
  points += used_signs[25] * 10 - 10;

  return points;
}


char * random_word(FILE * diction, char letter, long int * alfabet_p)
{
  int number, i;
  char words[120], *word = words; 
	
  rewind(diction);
  number = rand() % (*(alfabet_p + letter - 'a' + 1) - *(alfabet_p + letter - 'a'));
  for (i = 0; i <= number + *(alfabet_p + letter - 'a'); i++)
    {fgets(words, 120, diction);}
  word = (char *) malloc((strlen(words) + 1) * sizeof(*word));
  strncpy(word, words, strlen(words) - 1);
	
  return word;
}


char * random_pass(FILE * diction, long int * alfabet_p)
{
  int number;
  char * passwd;
	 
  rewind(diction);
  number = rand() % 25;
  passwd = random_word(diction, 'a' + number, alfabet_p);

  return passwd;
}


char ** random_cross(FILE * diction, long int * alfabet_p, char * passwd)
{
  int i;
  char* * cross;

  cross = (char **) malloc((strlen(passwd) - 1) * sizeof(char **));
  for (i = 0; i < strlen(passwd) - 1; i++)
    {  *(cross + i) = random_word(diction, passwd[i], alfabet_p);}
  free(passwd);
	
  return cross;		
}


void free_mem(int pass_leng, char ** cross)
{
  int i;
	
  for(i = 0; i < pass_leng - 1; i++)
    free(cross[i]);
  free(cross);
}


void get_sign(char *used_signs)
{
  char sign;
  char check;
	
  do {
    printf("\nPodaj litere (od 'a' do 'z'):");
    sign = getchar();
    if ((sign < 'a') || (sign > 'z'))
    {  
      printf("Blad, to nie litera! Wpisz ponownie! \n");
      check = 1;
    }
    else
      check = 0;
    while (getchar() != '\n')
    {}
  } while (check);
  
  if (*(used_signs + sign - 'a') > 0)
  {
    printf("Litera byla juz wybrana!\n");
    get_sign(used_signs);
  }
  else
    (*(used_signs + sign - 'a'))++;	
}


int show(int pass_leng, char **cross, char *used_signes)
{
  int i,j, end = 0;

  for(i = 0; i < pass_leng - 1; i++)
  { 
    if (i == 0)
    {
      for(j = 0; j < strlen(cross[i]) - 1; j++)
        {printf(" ___");}
      printf("\n");
    }
    for(j = 0; j < strlen(cross[i]) - 1; j++)
      {printf("|   ");}
    printf("|\n") ;
    for(j = 0; j < strlen(cross[i]) - 1; j++)
    {
      if (*(used_signes + cross[i][j] - 'a'))
      {
        printf("| %c ", cross[i][j]);
        (*(used_signes + cross[i][j] - 'a'))++;
      }
      else
      {
        printf("|   ");
        end++;
      }
    }
    printf("|\n");
    for(j = 0; j < strlen(cross[i]) - 1; j++)
      {printf("|___");}
    if (i > 0)
    {
      if (i < pass_leng - 2)
      {
        if (strlen(cross[i + 1]) - strlen(cross[i]) > 0) 
        {
          printf("|");
          for(j = strlen(cross[i]) - 1; j < strlen(cross[i + 1]) - 1; j++)
            {printf("___ ");}
          printf("\n");
        }
        else  
          printf("|\n");
      }
      else
        {printf("|\n"); }
    }
    else
    {
      if (i < pass_leng - 2)
      {
        if (strlen(cross[i + 1]) - strlen(cross[i]) > 0) 
        {
          printf("|");
          for(j = strlen(cross[i]) - 1; j < strlen(cross[i + 1]) - 1; j++)
            {printf("___ ");}
          printf("\n");
        }
        else
          printf("|\n");
      }
      else
        printf("|\n");
    }
  }
	
  return end;
}
