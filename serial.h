/*
 * serial.h
 * biblioteca com funcoes da comunicacao serial com o Arduino
 */

#ifndef SRC_SERIAL_H_
#define SRC_SERIAL_H_
#include "../definicoes.h"

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
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
