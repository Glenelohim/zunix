#include <stdio.h>
#include <signal.h>

int main()
{
	printf("begin\n");
	fork();
	printf("1\n");
	fork();
	printf("2\n");
	fork();
	printf("end\n");
	return 0;
}

/*
output: see how many fork before printf then dup2

begin ->  1 time
1     ->  2 times
2     ->  4 times
end   ->  8 times

*/