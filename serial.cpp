#include "serial.h"

int Serial::enviaByte(char b){
    int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;
}

int Serial::envia(const char* str){
    int len = strlen(str);
	int i=0;
    while(i<len){
	write(fd,&str[i],1);
	usleep(1000);
	i++;
	}
    return 0;
}

int Serial::envia(string str){
    return envia(str.c_str());
}

string Serial::recebeAteCaractere(char caractere){
	string buf ="";
	char b[1];
	int tentativas=0;
	int n;
	timeOut/=100;
	do {
	n = read(fd, b, 1);  

	if(n==0 || n==-1) {
		usleep(100*10); 
		if(tentativas++ > timeOut) {
			//retorna o que conseguiu ler
			return buf;
		}
		continue;
	}
	buf+= b[0];
	} while( b[0] != caractere );
	return buf;
}

string Serial::recebeAteTamanho(int tamanho){
	string buf;
    char b[1];
    int i=0;
    do {
        int n = read(fd, b, 1);   
        if( n==0  || n==-1) {
            usleep( 100 * 1000 ); 
            continue;
        }
        buf += b[0]; i++;
    } while( i < tamanho );
    return buf;
}

int Serial::inicializa(){
    struct termios toptions, toptions_old;
    memset (&toptions, 0, sizeof toptions);

    speed_t brate = baud; 
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }


    fd = open(porta.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }

    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }

	toptions_old = toptions;


    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;

    toptions.c_cflag |= CREAD | CLOCAL;  

    toptions.c_cc[VMIN]  = 1;
    toptions.c_cc[VTIME] =5;

    cfmakeraw(&toptions);
    tcflush(fd,TCIFLUSH);
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}

Serial::Serial(const char * porta, int baud){
	fd=0;
	timeOut = 100;
	this->porta = porta;
	this->baud = baud;
}

