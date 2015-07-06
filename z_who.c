#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>  //open
#include <unistd.h>	//read, close
#include <time.h>   //ctime

#define SHOWHOST  //remote host name

void show_who(struct utmp *utbufp);
void ut2ctime(long utime);

int main()
{
	struct utmp current_record;  //the info of users saved as this struct
	int utmpfd;
	int reclen = sizeof(current_record);

	if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 )
	{
		perror(UTMP_FILE);
		exit(1);
	}
	while (read(utmpfd, &current_record, reclen) == reclen)
	{
		show_who(&current_record);
	}
	close(utmpfd);

	return 0;
}

void show_who(struct utmp *utbufp)
{
	if (utbufp->ut_type != USER_PROCESS) //active user
		return;

	printf("%-8.8s ", utbufp->ut_name);  //user name
	printf("%-8.8s ", utbufp->ut_line);	 //terminal name
	
	ut2ctime(utbufp->ut_time);           //login time

	#ifdef SHOWHOST
	if (utbufp->ut_host[0] != '\0')		 //if exit
		printf(" (%s)", utbufp->ut_host);     //remote host name
	#endif

	printf("\n");
}

void ut2ctime(long utime)
{
	char *cp = ctime(&utime);
	printf("%12.12s", cp + 4);  //omit the week
}