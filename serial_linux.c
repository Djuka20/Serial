#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <termios.h>


const char* ime = "/dev/pts/4";

//vraca true ako citanje treba da se nastavi a false ako treba da se izadje
bool handle_data(char c){
	
	//printf("0x%02x\n", c);
		
	printf("%c", c);
		
	if(c == '\r'){
			
		printf("\n");
	}
		
	if(c == 0x1b){
	//proverava da li je ESCAPE
			
		return false;
	}
	
	return true;
}


int main(){
	
	//  Creates the file to read 
	int port_handle = open(ime, // file name
                         O_RDWR // open for read and write
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
	
	
	
	
	while(true){
		
		char buffer[1024];
		
		int procitano = 0;
		procitano = read(port_handle, buffer, 1024); 
		
		if(procitano <= 0){
			
			break;
		}
		
		for(int i = 0; i < procitano; i++){
		
			bool ok = handle_data(buffer[i]);
			
			if(!ok){
				
				goto end_of_while;
			}
		}
		
		printf("\n--------------------\n");
	}	
end_of_while:	
	
	
	
	
	close(port_handle);
	port_handle = -1;
	
	return 0;
}

