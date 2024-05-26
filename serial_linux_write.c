#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <termios.h>


const char* ime = "/dev/pts/3";




int main(){
	
	//  Creates the file to read 
	int port_handle = open(ime, // file name
                         O_RDWR | O_ASYNC | O_NDELAY // open for read and write
                        );
	
	
	if(port_handle < 0){
		printf("greska u otvaranju fajla %s\n", ime);
		
		return 1;
	}
	
	printf("port %s otvoren\n", ime);
	
	
	//read stdin witout enter key
	struct termios newt;

    tcgetattr(port_handle, &newt);//get atributtes

    newt.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                           | INLCR | IGNCR | ICRNL | IXON);
	newt.c_oflag &= ~OPOST;
    newt.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    newt.c_cflag &= ~(CSIZE | PARENB);
    newt.c_cflag |= CS8;
	
	cfmakeraw(&newt);
	
	newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 10;

    newt.c_cflag &= ~CSTOPB;
    newt.c_cflag &= ~CRTSCTS;    /* no HW flow control? */
	newt.c_cflag |= CLOCAL | CREAD;
	
    tcsetattr(port_handle, TCSANOW, &newt);//set atributtes
	//TODO: restore origin state
	
	char* message = "Marko voli fudbal";
	size_t message_len = strlen(message);
	
	int upisano = write(port_handle, message, message_len);
	
	if(upisano != message_len){
		
		printf("greska pri upisu\n");
	}	
	
	close(port_handle);
	port_handle = -1;
	
	return 0;
}

