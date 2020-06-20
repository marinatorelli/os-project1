#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <sys/types.h>          /* Cabeceras llamada al sistema open  */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>             /* Cabecera llamadas al sistema read, write y close  */


int main(int argc, char *argv[])
{
	/*The program has to always receive only one input*/
	if (argc != 2) {
		printf("The input isn't valid\n");
		return -1;
	}	
	/*We open the file with O_RDONLY to read, as we don't need to write into the file*/
	int fileDesc = open(argv[1], O_RDONLY);
	/*If there was an error in opening the file the syscall returns -1*/
	if (fileDesc < 0) {
		perror("Error in opening the file");
		return -1;
	}
	int nBytes;
	char buf[1024]; /*We consider a buffer of size 1024*/
	while ((nBytes = read(fileDesc, buf, sizeof(buf))) > 0) { /*We store the read bytes into the buf and the number of bytes read*/
		if ((write (STDOUT_FILENO, buf, nBytes)) != nBytes) { /*We write the number of bytes read stored in the buffer, if the number of bytes written is different from the ones read there was an error*/
			perror("Error in writing");
			return -1;
		}
	}
	/*If the read syscall returned -1 there was an error, if it stopped at 0 it was a success*/
	if (nBytes < 0) {
		perror("Error in reading");
		return -1;
	}
	/*The close syscall returns -1 if there was an error*/
	if ((close(fileDesc)) < 0) {
		perror("Error in closing the file");
		return -1;
	}
	return 0;
}
