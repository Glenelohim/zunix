#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFSIZE 4096

void printerror(const char *s1, const char *s2);

int main(int argc, const char *argv[])
{
	int in_fd, out_fd, n_chars;
	char buff[BUFFSIZE];
	struct stat file_info;

	if (argc != 3)
	{
		fprintf(stderr, "%s source destination\n", *argv);
		exit(1);
	}
	if ((in_fd = open(argv[1], O_RDONLY)) == -1)
		printerror("Cannot open ", argv[1]);
	
	//read the file mode
	if (fstat(in_fd, &file_info) == -1)
		printerror("Error read file ", argv[1]);

	//the same mode as the source file
	if ((out_fd = creat(argv[2], file_info.st_mode)) == -1)
		printerror("Cannot create ", argv[2]);

	while ((n_chars = read(in_fd, buff, BUFFSIZE)) > 0)
		if (write(out_fd, buff, n_chars) != n_chars)
			printerror("Write error to ", argv[2]);

	if (n_chars == -1)
		printerror("Read error from ", argv[1]);

	if ( (close(in_fd) == -1) || (close(out_fd) == -1))
		printerror("Error closing from ", "");

	return 0;
}

void printerror(const char *s1, const char *s2)
{
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}