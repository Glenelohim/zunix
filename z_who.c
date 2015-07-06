#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST  //remote host name
#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int  num_recs;
static int  cur_rec;
static int  fd_utmp = -1;

int utmp_open(const char *filename);
struct utmp *utmp_next();
int utmp_reload();
void utmp_close();
void show_who(struct utmp *utbufp);
void ut2ctime(long utime);

int main()
{
	struct utmp *current_record;  //the info of users saved as this struct
	struct utmp *utmp_next();

	if (utmp_open(UTMP_FILE) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}
	while ((current_record = utmp_next()) != (struct utmp *)NULL)
		show_who(current_record);

	utmp_close();

	return 0;
}

int utmp_open(const char *filename)
{
	fd_utmp = open(filename, O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}

struct utmp *utmp_next()
{
	struct utmp *recp;
	if (fd_utmp == -1)
		return NULLUT;
	if (cur_rec == num_recs && utmp_reload() == 0)
		return NULLUT;

	recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
	return recp;
}

//read next bunch of records into buffer
int utmp_reload()
{
	int amt_read;
	amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
	num_recs = amt_read / UTSIZE;
	cur_rec = 0;
	return num_recs;
}

void utmp_close()
{
	if (fd_utmp != -1)
		close(fd_utmp);
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