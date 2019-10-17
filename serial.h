/*
 * serial.h
 * biblioteca com funcoes da comunicacao serial com o Arduino
 */

#ifndef SRC_SERIAL_H_
#define SRC_SERIAL_H_
#include "../definicoes.h"

#include <stdio.h>   
#include <stdlib.h>
#include <stdint.h>  
#include <string.h>  
#include <unistd.h> 
#include <fcntl.h>   
#include <errno.h>    
#include <termios.h>  
#include <sys/ioctl.h>
#include <getopt.h>
#include <time.h>
#include <pthread.h>

using namespace std;

class Serial{
private:
	int fd;
	char verificacao[1000];
	int baud;
	string porta;
	int timeOut; //timeOut em milissegundos
public:
	Serial(const char * porta, int baud);
	int enviaByte(char b);
	int envia(const char* str);
	int envia(string str);
	int inicializa();
	string recebeAteCaractere(char caractere);
	string recebeAteTamanho(int tamanho);

	void setTimeOut(int t){
		timeOut = t;
	}
	int getTimeOut(){
		return this->timeOut;
	}
	void limpaBuffer(){
		tcflush(fd,TCIFLUSH);
	}

};

#endif /* SRC_SERIAL_H_ */
