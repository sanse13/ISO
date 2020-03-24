#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/ioctl.h>

//configura el estado de la terminal asignado a la entrada estandar
//a modo no canonico.
void setTermNoCanon(struct termios *savedTM){
    struct termios tm;
    tcgetattr(0, &tm);
    tcgetattr(0, savedTM);
    tm.c_lflag &= ~(ICANON|ECHO);
    tm.c_cc[VMIN] = 1;
    tm.c_cc[VTIME]= 0;
    tcsetattr(0, TCSANOW, &tm);
}

//recupera el estado de la terminal a un estado previo
void restoreTerm(struct termios *savedTM){
    tcsetattr(0, TCSANOW, savedTM);
}

void eco(struct termios tm){
    ioctl(0, TCGETA, tm);
    if(tm.c_lflag& ECHO)
        tm.c_lflag &= ~ECHO;
    else
        tm.c_lflag |= ECHO;
    ioctl(0, TCSETA, &tm);
}