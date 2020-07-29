// SO2 IS1 211A LAB04
// Radosław Bylica
// br41338@zut.edu.pl
// gcc 41338.so2.lab04.main.c -o lab04
// ./lab04 abcde
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
{	//blokowanie sygnałów
	sigset_t blokada;
	sigaddset(&blokada, SIGTSTP); //crtl z
	sigaddset(&blokada, SIGINT); //crtl c
	sigprocmask(SIG_BLOCK, &blokada, NULL);

	int licznik = 0;
	sigset_t kontener;

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
		int child_pid1 = fork();
		if(child_pid1 > 0)
		{
			int child_pid2 = fork();
			if(child_pid2 > 0)
			{
				while(1)
				{
					if(!(sigpending(&kontener)))
					{ 
						if(sigismember(&kontener, SIGINT))
						{
							if(licznik == 1)
								printf("\nCrtl+Z uzyto przez %d.\n\n", getpid());
							kill(child_pid1,SIGINT);
							kill(child_pid2,SIGINT);
							break;
						}
						if(sigismember(&kontener, SIGTSTP))
						{
							if(licznik != 1)
								licznik = 1;
							sigdelset(&kontener, SIGTSTP);
						}
					}
				}
				int *status;
				wait(status);
				wait(status);
		
				printf("%d ", getpid());
				printf("%s \n", droga);

				exit(0);
			}
			else
			{
				setpgid(child_pid2,0); //zmienia grupe

				strcat(droga, " ");
				strcat(droga, tab_lewo);
				strcat(droga, "\0");

				execl(argv[0], argv[0], tab_lewo, droga, NULL);
			}
		}
		else
		{
			setpgid(child_pid1,0); //zmienia grupe

			strcat(droga, " ");
			strcat(droga, tab_prawo);
			strcat(droga, "\0");

			execl(argv[0], argv[0], tab_prawo, droga, NULL);
		}
	}
	else
	{
		while(1)
		{
			if(!(sigpending(&kontener)))
			{ 
				if(sigismember(&kontener, SIGINT))
				{
					break;
				}
			}
		}
		printf("%d ", getpid());
		printf("%s \n", droga);

		free(tab_lewo);
		free(tab_prawo);
		free(droga);

		exit(0);
	}
	free(tab);
}
return 0;
}
