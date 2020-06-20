#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>				/*Header to accesss the strcmp function*/
#include <errno.h>				/*Header to acccess the value of errno*/


int main(int argc, char *argv[])
{
	/*The mysize function doesn't receive any input*/
	if (argc != 1) {
		printf("The input isn't valid\n");
		return -1;
	}
	DIR* dir;
	char buf[PATH_MAX];
	/*If there is an error getcwd returns NULL*/
	if ((getcwd(buf, PATH_MAX)) == NULL) {
		perror("Error in current directory");
		return -1;
	}
	dir = opendir(buf);
	/*If the opendir function returned NULL there was an error in opening the directory*/
	if (dir == NULL) {
		perror("Error in opening directory");
		return -1;
	}
	struct dirent *entry;
	/*To check if the readdir function returns an error we have to store the previous value of errno*/
	int checkError = errno;
	while ((entry=readdir(dir)) != NULL) {
		/*With the rest of the entries we check if it is a regular file, the type DT_REG stored in d_type of the dirent structure*/
		if ((entry->d_type) == DT_REG) {
			/*We open the file to read only*/
			int fileDesc = open(entry->d_name, O_RDONLY);
			/*If the file descriptor is -1 here was a problem in the open syscall*/
			if (fileDesc < 0) {
				perror("Error in opening the file");
				return -1;
			}
			int fileSize;
			/*We use lseek to put the pointer at the end of the file (SEEK_END) with an offset of zero and store the value returned, which will be the number of bytes from the beginning of the file to the pointer*/
			fileSize = lseek(fileDesc, 0, SEEK_END);
			/*If there was an error lseek returns -1*/
			if (fileSize < 0) {
				perror("Error in calculating size of file");
				return -1;
			}
			/*We close the file, checking if the value returned is -1, in which case there was an error*/
			if ((close(fileDesc)) < 0) {
				perror("Error in closing the file");
				return -1;
			}
			/*We print the name of the file, a tab and the size*/
			printf("%s\t%d\n", entry->d_name, fileSize);
		}
	}
	/*We check if there was any error in readdir, in which case the value of errno would have changed, so we compare it with the one previously stored*/
	if (errno != checkError) {
		perror("Error in reading entries of directory");
		return -1;
	}
	/*If there was an error in closedir it returns -1*/
	if ((closedir(dir)) < 0) {
		perror("Error in closing directory");
		return -1;
	}
	return 0;
}
