#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

//#define DELAY 2
#define MAXARGS 20
#define ARGLEN	100

int main(int argc, char const *argv[])
{
	/* code */

	/*
	==============
	step1: test for & understand fork()
	==============
	*/
	/*
	void child_code(int);
	void parent_code(int);

	int newpid;
	printf("before: my pid is %d\n", getpid());

	if ((newpid = fork()) == -1)
		perror("fork failed!");
	else if (newpid == 0)
		child_code(DELAY);
	else
		parent_code(newpid);
	*/

	/*
	==========
	step2: shell demo1
	==========
	*/ 
	char* arglist[MAXARGS + 1];
	int numargs;
	char argbuf[ARGLEN];

	char* makestring(char*);
	void execute();

	numargs = 0;
	while (numargs < MAXARGS)
	{
		printf("arg[%d]? ", numargs);
		if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
			arglist[numargs++] = makestring(argbuf);
		else
		{
			if (numargs > 0)
			{
				arglist[numargs] = NULL;
				execute(arglist);
				numargs = 0;
			}
		}
	}

	return 0;
}

/*
	==========
	step2: shell demo1
	==========
*/
void execute(char* arglist[])
{
	int pid, exitstatus;

	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(1);
		case 0: //child
			execvp(arglist[0], arglist);
			perror("execvp failed");
			exit(1);
		default: //parent
			while (wait(&exitstatus) != pid)
				;
			printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus&0x0377);
	}
}

char* makestring(char* buf)
{
	char* cp;
	buf[strlen(buf) - 1] = '\0';
	cp = malloc(strlen(buf) + 1);
	if (cp == NULL)
	{
		perror("no memory");
		exit(1);
	}
	strcpy(cp, buf);
	return cp;
}

/*
	step1: test for & understand fork()
*/
/*
void child_code(int delay)
{
	printf("child %d here will sleep %d seconds\n", getpid(), delay);
	sleep(DELAY);
	printf("child done\n");
	exit(17);
}

void parent_code(int childpid)
{
	int wait_status;
	int wait_rv = wait(&wait_status);
	printf("done waiting for %d wait returned %d\n wait_status: %d\n", childpid, wait_rv, wait_status>>8);
}
*/