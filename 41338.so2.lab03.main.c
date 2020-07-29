// SO2 IS1 211A LAB03
// Radosław Bylica
// br41338@zut.edu.pl
// gcc 41338.so2.lab03.main.c -o lab03
// ./lab03 abcde
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h> 
int main(char argc, char **argv)
{
if(argc < 2)
{
	printf("Uruchomniono program bez argumentow.\n\n");
}
else
{
	char * tab;
	int rozmiar = strlen(argv[1]);
	
	char * tab_lewo;
	char * tab_prawo;
	
	char * droga;
	int rozmiar_drogi;
	
	int petla;
	if(argc == 2)
	{
		int potega = 1;
		while(1) //ustalanie potegi argumentu
		{
			if(rozmiar == potega) //tablica ma rozmiar potegi
			{
				break;
			}
			else if(rozmiar < (potega * 2)) //rozmiar zostal zmieniony na potege
			{
				rozmiar = potega;
				break;
			}
		potega = potega * 2;
		} //end while
		
		tab = (char*) malloc ( (sizeof(char) * rozmiar) + sizeof(char) );
		for(petla = 0; petla < rozmiar; petla++)
			tab[petla] = argv[1][petla];
		strcat(tab, "\0");
		
		//inicjowanie drogi
		droga = (char*) malloc ( (sizeof(char) * rozmiar * (rozmiar / 2) ) + sizeof(char) );
		for(petla = 0; petla < rozmiar; petla++)
			droga[petla] = tab[petla];
		strcat(droga, "\0");
		printf("%d ", getpid());
		printf("%s \n", droga);
	}
	else if(argc == 3) 
	{
		tab = (char*) malloc ( (sizeof(char) * rozmiar) + sizeof(char) );
		for(petla = 0; petla < rozmiar; petla++)
			tab[petla] = argv[1][petla];
		
		rozmiar_drogi = strlen(argv[2]);
		droga = (char*) malloc ( (sizeof(char) * rozmiar_drogi * (rozmiar_drogi / 2) ) + sizeof(char));
		for(petla = 0; petla < rozmiar_drogi; petla++)
			droga[petla] = argv[2][petla];
		strcat(droga, "\0");
	}	
	
	tab_lewo = (char*) malloc ( ( (rozmiar / 2) * sizeof(char)) + sizeof(char) );
	tab_prawo = (char*) malloc ( ( (rozmiar / 2) * sizeof(char)) + sizeof(char) );
	for (petla = 0; petla < (rozmiar / 2); petla++)
	{
		tab_lewo[petla] = tab[petla]; //lewa strona tablicy
		tab_prawo[petla] = tab[petla + ( rozmiar / 2 )]; //prawa strona tablicy
	}
	strcat(tab_lewo, "\0");
	strcat(tab_prawo, "\0");
	
	if (rozmiar > 1)
	{
		int child_pid = fork();
		if(child_pid > 0)
		{
			int child_pid = fork();
			if(child_pid > 0)
			{
				int *status;
				wait(status);
				wait(status);
				exit(0);
			}
			else
			{
				strcat(droga, " ");
				strcat(droga, tab_lewo);
				strcat(droga, "\0");
				
				printf("%d ", getpid());
				printf("%s \n", droga);
				
				execl(argv[0], argv[0], tab_lewo, droga, NULL);
			}
		}
		else
		{
			strcat(droga, " ");
			strcat(droga, tab_prawo);
			strcat(droga, "\0");
			
			printf("%d ", getpid());
			printf("%s \n", droga);
			execl(argv[0], argv[0], tab_prawo, droga, NULL);
		}
	}
	else
	{
		free(tab_lewo);
		free(tab_prawo);
		free(droga);
		exit(0);
	}
	free(tab);
}
return 0;
}
