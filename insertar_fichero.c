#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include "util.h"

#define E_OPEN "No se puede abrir file_mypackzip\n"
#define ERR_OPEN 1

#define E_POS "Número de registro erróneo (Posicion)\n"
#define ERR_POS 2

#define E_OPEN2 "No se puede abrir  fich_origen.\n"
#define ERR_OPEN2 3

#define TAM 5 //TAM FICHEROS??

//insertar con el header??

int insertar_fichero(char * fich_origen, long Posicion, char * file_mypackzip){

    struct s_header header;

    int sourceFileID = open(fich_origen, O_RDONLY, 0644);
    int destFileID = open(file_mypackzip, O_CREAT | O_RDWR, 0644); //el comprimido

    int n;
    int actual_reg;
    char buf[TAM_BUFFER];

    strcpy(header.InfoF.FileName, fich_origen);
    header.InfoF.Tipo = 'Z';
    header.InfoF.Compri = 'N';
    header.InfoF.TamOri = lseek(sourceFileID, 0L, SEEK_END);
    header.InfoF.TamComp = header.InfoF.TamOri;

    lseek(sourceFileID, 0L, SEEK_SET);
    

    //insertando al final
    if (Posicion == -1 || Posicion > TAM-1){ //inserta al final

    lseek(destFileID, 0L, SEEK_END);

    write(destFileID, &header, sizeof(header));

    while ( (n = read(sourceFileID, buf, TAM_BUFFER)) > 0)
        write(destFileID, buf, n);

    close(sourceFileID);
    close(destFileID);

    return 0;

    } else { //insertar en Posicion. 

        
    }

}
