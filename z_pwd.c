#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define BUFFSIZE 100

ino_t getInode(char *filename);
void ino2fname(ino_t ino, char *namebuff, int bufflen);
void printPath(ino_t inode);


int main(int argc, char const *argv[])
{
	/* code */
	printPath(getInode("."));
	printf("\n");
	return 0;
}

//return the inode of file from its name
ino_t getInode(char *filename)
{
	struct stat info;
	if (stat(filename, &info) == -1)
	{
		fprintf(stderr, "Cannot stat");
		perror(filename);
		exit(-1);
	}
	return info.st_ino;
}

//search for the file from its inode number
void ino2fname(ino_t ino, char *namebuff, int bufflen)
{
	DIR *dir_ptr;			  //in this dir
	struct dirent *dirent_p;  //each item
	dir_ptr = opendir(".");   //current dir
	if (dir_ptr == NULL)
	{
		perror(".");
		exit(-1);
	}
	while ((dirent_p = readdir(dir_ptr)) != NULL) //search for the ino in this dir
	{
		if (dirent_p->d_ino == ino)
		{
			strncpy(namebuff, dirent_p->d_name, bufflen);
			namebuff[bufflen - 1] = '\0';
			closedir(dir_ptr);
			return;
		}
	}
	fprintf(stderr, "cannot search for the inode");
	exit(1);
}

void printPath(ino_t inode)
{
	ino_t inoR;
	char inode_name[BUFFSIZE];
	if (getInode("..") != inode)  //until the '/' -> "."=='..'
	{
		chdir(".."); //enter its parentDir
		ino2fname(inode, inode_name, BUFFSIZE);
		inoR = getInode(".");  //change the current dir
		printPath(inoR);  //recursive
		printf("/%s", inode_name);
	}
}