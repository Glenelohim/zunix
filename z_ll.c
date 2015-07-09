#include <stdio.h>
#include <string.h>
#include <sys/types.h> //system data type
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>  //uid,uname
#include <grp.h>  //group

void list(const char dirname[]);
void list_stat(const char *filename);
void show_file_info(const char *filename, struct stat *file_info);
void mode2letter(int mode, char *str);
char *uid2name(uid_t uid);
char *gid2name(gid_t gid);

int main(int argc, const char *argv[])
{
	/* code */
	if (argc == 1)
		list(".");
	else
	{
		while (--argc)
		{
			printf("%s: \n", *(++argv));
			list(*argv);
		}
	}
	return 0;
}

void list(const char *dirname)
{
	DIR *dir_ptr;  //the directory
	struct dirent *dirent_p; //each item

	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "Can not open %s\n", dirname);
	else
	{
		while ((dirent_p = readdir(dir_ptr)) != NULL)
			list_stat(dirent_p->d_name);
		closedir(dir_ptr);
	}
}

void list_stat(const char *filename)
{
	struct stat file_info;
	if (stat(filename, &file_info) == -1)
		perror(filename);
	else
		show_file_info(filename, &file_info);
}

void show_file_info(const char *filename, struct stat *file_info)
{
	char modestr[11];
	mode2letter(file_info->st_mode, modestr);
	modestr[10] = '\0';

	printf("%s", modestr);
	printf("%4d", (int)file_info->st_nlink);
	printf(" %-8s", uid2name(file_info->st_uid));
	printf("%-8s", gid2name(file_info->st_gid));
	printf("%4ld", (long)file_info->st_size);
	printf(" %.12s", (char *)4 + ctime(&file_info->st_mtime));
	printf(" %s\n", filename);  //could add color here by judging from the file type
}

//the mode of file
void mode2letter(int mode, char *str)
{
	strncpy(str, "----------", 10);
	//file mode
	if (S_ISDIR(mode))
		str[0] = 'd';
	if (S_ISCHR(mode))
		str[0] = 'c';
	if (S_ISBLK(mode))
		str[0] = 'b';
	//user mode
	if (mode & S_IRUSR)
		str[1] = 'r';
	if (mode & S_IWUSR)
		str[2] = 'w';
	if (mode & S_IXUSR)
		str[3] = 'x';
	//group mode
	if (mode & S_IRGRP)
		str[4] = 'r';
	if (mode & S_IWGRP)
		str[5] = 'w';
	if (mode & S_IXGRP)
		str[6] = 'x';
	//other mode
	if (mode & S_IROTH)
		str[7] = 'r';
	if (mode & S_IWOTH)
		str[8] = 'w';
	if (mode & S_IXOTH)
		str[9] = 'x';
}

//return uid
char *uid2name(uid_t uid)
{
	struct passwd *getpwuid();
	struct passwd *pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL)
	{
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

//return group number
char *gid2name(gid_t gid)
{
	struct group *getgrgid();
	struct group *grp_ptr;
	static char numstr[10];

	if ((grp_ptr = getgrgid(gid)) == NULL)
	{
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}