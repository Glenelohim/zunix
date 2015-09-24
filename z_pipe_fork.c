#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHILD_MESS "I want a cookie\n"
#define PAR_MESS "testing..\n"
#define BUFFSIZE 512
#define oops(m, x) {perror(m); exit(x);}

int main(int argc, char const *argv[])
{
	/* code */
	int pipefd[2];
	int len;
	char buf[BUFFSIZE];
	int read_len;

	if (pipe(pipefd) == -1)
		oops("cannot get a pipe", 1);

	switch (fork())
	{
		case -1:
			oops("cannot fork", 2);

		case 0://child
			len = strlen(CHILD_MESS);
			while (1)
			{
				if (write(pipefd[1], CHILD_MESS, len) != len)
					oops("child write error", 3);
				sleep(5);
			}

		default://parent
			len = strlen(PAR_MESS);
			while (1)
			{
				if (write(pipefd[1], PAR_MESS, len) != len)
					oops("parent write error", 4);
				sleep(1);
				read_len = read(pipefd[0], buf, BUFFSIZE);
				if (read_len <= 0) //finish read
					break;
				write(1, buf, read_len);
			}
	}

	return 0;
}