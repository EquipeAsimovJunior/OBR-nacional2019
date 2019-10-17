/*
 * Arduino-serial
 * --------------
 *
 * A simple command-line example program showing how a computer can
 * communicate with an Arduino board. Works on any POSIX system (Mac/Unix/PC)
 *
 *
 * Compile with something like:
 * gcc -o arduino-serial arduino-serial.c
 *
 * Created 5 December 2006
 * 2006, Tod E. Kurt, tod@todbot.com
 * http://todbot.com/blog/
 *
 */
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
//    int n = write(fd, str, len);
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
	//fcntl(fd, F_SETFL, 0);
	timeOut/=100;
	do {
	n = read(fd, b, 1);  // read a char at a time
	/*if( n==3){
		return -1;    // couldn't read
	 }*/
	if(n==0 || n==-1) {
		usleep(100*10); // wait 1 msec try again
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
        int n = read(fd, b, 1);  // read a char at a time
       // if( n==-1) return -1;    // couldn't read
        if( n==0  || n==-1) {
            usleep( 100 * 1000 ); // wait 100 msec try again
            continue;
        }
        buf += b[0]; i++;
    } while( i < tamanho );
    return buf;
}

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
int Serial::inicializa(){
    struct termios toptions, toptions_old;
    memset (&toptions, 0, sizeof toptions);
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);

    speed_t brate = baud; // let you override switch below if needed
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

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    //toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    //toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    //toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    //toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
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

