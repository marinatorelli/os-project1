#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <unistd.h>             /* Cabecera llamada al sistema gtcwd  */
#include <sys/types.h>          /* Cabeceras llamadas al sistema opendir, readdir y closedir  */
#include <dirent.h>
#include <errno.h>				/*Header to acccess the value of errno*/


int main(int argc, char *argv[])
{
	/*We can either receive one or no arguments*/
	if (argc != 1 && argc != 2) {
		printf("The input isn't valid\n");
		return -1;
	}
	DIR* dir;
	/*If we receive an input in argv[1] we consider it the path of a directory so we use the opendir syscall*/
	if (argv[1] != NULL) {
		dir = opendir(argv[1]);
	} else {
		/*If we don't receive any input we get the path of the current directory using the getcwd syscall*/
		char buf[PATH_MAX]; /* We use the constant PAHT_MAX as size of the buffer*/
		if (getcwd(buf, PATH_MAX)==NULL) { /*If there is an error getcwd returns NULL*/
			perror("Error in current directory");
			return -1;
		}; 
		/*If there is no error in getcwd we will have the path in buf, so we can call the opendir syscall*/
		dir = opendir(buf);
	}
	/*If the opendir function returned NULL there was an error in opening the directory*/
	if (dir == NULL) {
		perror("Error in opening directory");
		return -1;
	}
	struct dirent *entry;
	/*To check if the readdir function returns an error we have to store the previous value of errno*/
	int checkError = errno;
	/*We read the entries of the directory until it either finishes or finds an error, either way the readdir syscall returns NULL*/
	while ((entry=readdir(dir)) != NULL) {
		/*We print the name of the entry stored in the d_name field of the dirent structure*/
		printf("%s\n", entry->d_name);
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