//// SO2 IS1 211A LAB05
// Radosław Bylica
// br41338@zut.edu.pl
// gcc 41338.so2.lab05.main.c -o lab05
// ./lab05 -dflL2n
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"
void tree(char *droga, int *flagi, int glebokosc)
{
	int petla;

	DIR *katalog = opendir(droga);
	struct dirent *teraz;
	
	if (katalog)
	{
		while ((teraz = readdir(katalog)) != NULL) 
		{
			if ((strcmp(teraz->d_name, ".")) && (strcmp(teraz->d_name, "..")))
			{
				if(teraz->d_type == DT_DIR)
				{
					printf("|");
					for(petla = 0; petla < glebokosc; petla++)
						printf("-");
					if(flagi[2] == 1)
						printf(GRN "%s/" RESET, droga);
					printf(GRN "%s\n" RESET, teraz->d_name);
					if(flagi[3] > glebokosc || flagi[3] == 0)
					{
						char *droga_t =(char*) malloc(sizeof(char) * strlen(droga) + sizeof(char) * strlen(teraz->d_name) + sizeof(char) * 2);
						strcpy(droga_t, droga);
						strcat(droga_t, "/");
						strcat(droga_t, teraz->d_name);

						tree(droga_t, flagi, glebokosc+1);
						free(droga_t);
					}
				}
				else if(teraz->d_type == DT_LNK && flagi[0] == 0) 
				{
					printf("|");
					for(petla = 0; petla < glebokosc; petla++)
						printf("-");
					if(flagi[2] == 1)
						printf(BLU "%s/" RESET, droga);
					printf(BLU "%s\n" RESET, teraz->d_name);

					if((flagi[3] > glebokosc || flagi[3] == 0) && flagi[1] == 1)
					{
						char *droga_t =(char*) malloc(sizeof(char) * strlen(droga) + sizeof(char) * strlen(teraz->d_name) + sizeof(char) * 2);
						strcpy(droga_t, droga);
						strcat(droga_t, "/");
						strcat(droga_t, teraz->d_name);

						tree(droga_t, flagi, glebokosc+1);
						free(droga_t);
					}
				}
				else if(flagi[0] == 0) 
				{
					printf("|");
					for(petla = 0; petla < glebokosc; petla++)
						printf("-");
					if(flagi[2] == 1)
						printf("%s/", droga);
					printf("%s\n", teraz->d_name);
				}			
			}
		}
	closedir(katalog);
	}
	else
	{
		printf("|");
		for(petla = 0; petla < glebokosc; petla++)
			printf("-");
		printf("Nie udalo sie otworzyc %s.\n", droga);
	}
}

int main(char argc, char **argv)
{
int petla;

int ilosc_flagi = 5;
int *flagi = (int*)malloc(sizeof(int) * ilosc_flagi);
for(petla = 0; petla < ilosc_flagi; petla++)
	flagi[petla] = 0; //0 = d, 1 = l, 2 = f, 3 = L, 4 = n
flagi[0] = 0; flagi[1] = 0; flagi[2] = 0; flagi[3] = 0; 

char *nazwa_katalogu = (char*) malloc (sizeof(char) * 2);
strcat(nazwa_katalogu, ".");

int opt;
while((opt = getopt(argc, argv, "dlfL:n:")) != -1)
{
	switch(opt) 
	{
	case 'd': flagi[0] = 1; break;
	case 'l': flagi[1] = 1; break;
	case 'f': flagi[2] = 1; break;
	case 'L': flagi[3] = atoi(optarg); break;
	case 'n': 
		flagi[4] = 1; 
		free(nazwa_katalogu);
		nazwa_katalogu = (char*) malloc (sizeof(char) * strlen(optarg) + sizeof(char));
		strcat(nazwa_katalogu, optarg);
		strcat(nazwa_katalogu, "\0");
		break;
	case '?': 
	if(optopt == 'n')
		printf("\nArgument 'n' wymaga nazwy katalogu.\n");
	else if(optopt == 'L')
		printf("\nArgument 'L' wymaga liczby.\n");
	else
		printf("\nNie rozpoznano argumentu.\n");
	return 0;
	}
}

printf(RED "%s\n" RESET, nazwa_katalogu);	
tree(nazwa_katalogu, flagi, 1);


free(flagi);
free(nazwa_katalogu);
return 0;
}
