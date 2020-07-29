// SO2 IS1 211A LAB01
// Radosław Bylica
// br41338@zut.edu.pl
// gcc 41338.so2.lab01.main.c -o lab01
// ./lab01 -ma
#include <stdlib.h>
#include <stdio.h>
#include <utmp.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
int main(int argc, char **argv)
{
int przelacznik;
int m = 0, a = 0;
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

setutent();
struct utmp * struktura = getutent();
char * user;
char * host;

printf("Uzytkownicy:\n\n");
while (struktura != NULL)
{
	user = struktura->ut_user;
	printf("%s", user);
	if (m == 1)
	{
		host = struktura->ut_host;
		printf(" (%s)", host);
	}
	
	if (a == 1)
	{
		printf(" [");
		int ngroups = 16;
		gid_t* groups = (gid_t*) malloc(ngroups * sizeof(gid_t));
		struct passwd* pw = getpwnam(user);
		if (!(pw == NULL))
		{
			if(getgrouplist (pw->pw_name, pw->pw_gid, groups, &ngroups) < 0)
			{
				free(groups);
				gid_t* groups = (gid_t*) malloc (ngroups * sizeof(gid_t));
				getgrouplist (pw->pw_name, pw->pw_gid, groups, &ngroups);
			}
			int i; 
			for (i = 0; i<ngroups; ++i)
			{
				int j = 0;
				struct group * grupa = getgrgid(groups[i]);
				while (NULL != grupa->gr_mem[j])
				{
					if ( !(strcmp(user, grupa->gr_mem[j++])) )
					{
						printf("%s ", grupa->gr_name);
						break;
					}
				}
			}
			free(groups);
		}
		printf("]");
	}
	printf("\n");
	struktura = getutent();
}
printf("\n\n");
return (0);
}