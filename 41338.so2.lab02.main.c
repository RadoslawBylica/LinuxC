// SO2 IS1 211A LAB02
// Radosław Bylica
// br41338@zut.edu.pl
// gcc -c -fPIC 41338.so2.lab02.lib.c; gcc -shared -fPIC 41338.so2.lab02.lib.o -o 41338.so2.lab02.lib.so; gcc 41338.so2.lab02.main.c -o lab02 -ldl;
// ./lab02 -ma
#include <stdlib.h>
#include <stdio.h>
#include <utmp.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <dlfcn.h>
#include "41338.so2.lab02.lib.h"
char* (*Grupy)(char * user);
int main(int argc, char **argv)
{
	int przelacznik;
	int m = 0, a = 0, error = 0;
	while ((przelacznik = getopt(argc, argv, "ma")) != -1)
		switch(przelacznik){
		case 'm':
			m = 1;
			break;
		case 'a':
			a = 1;
			break;
		default:
			printf("Nie poprawny przelacznik\n");
			break;
		}
	void *handle = dlopen( "./41338.so2.lab02.lib.so", RTLD_LAZY);
	if( !handle)
	{
		dlerror();
		error = 1;
	}
	else Grupy = dlsym( handle, "grupy");

	setutent();
	struct utmp * struktura = getutent();
	char * user;
	char * host;
	
	printf("Uzytkownicy:\n\n");
	while (struktura != NULL)
	{
		user = struktura->ut_user;
		if (struktura->ut_type == USER_PROCESS)
		{
			printf("%s", user);
			if ((m == 1) && (error == 0))
			{
				host = struktura->ut_host;
				printf(" (%s)", host);
			}
			if ((a == 1) && (error == 0))
			{
				char * temp = Grupy(user);
				printf("%s",temp);
			}
			printf("\n");
		}
	struktura = getutent();
	}
dlclose( handle);
return 0;
}
