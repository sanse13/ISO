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

#define TAM 15 //TAM FICHEROS??
//insertar con el header??

int insertar_fichero(char * fich_origen, long Posicion, char * file_mypackzip){

    struct s_header header;
    struct s_header header2;

    int sourceFileID = open(fich_origen, O_RDONLY);
    int destFileID = open(file_mypackzip, O_CREAT | O_RDWR, 0777); //el comprimido

    if (sourceFileID == -1){
        close(sourceFileID);
        write(2, E_OPEN2, strlen(E_OPEN2));
        return(ERR_OPEN2);
    }

    if (destFileID == -1){
        close(destFileID);
        write(2, E_OPEN, strlen(E_OPEN));
        return(ERR_OPEN);
    }

    if (Posicion < -1 || Posicion > TAM){
        close(sourceFileID);
        close(destFileID);
        write(2, E_POS, strlen(E_POS));
        return(ERR_POS);
    }

    int auxFileID;
    static char template[] = "/tmp/myFileXXXXXX";
    char fname[1024];
    strcpy(fname, template);
    auxFileID = mkstemp(fname);

    int n, n_aux, i;
    int actual_reg;
    char buf[TAM_BUFFER];

    while ( (n = read(destFileID, buf, TAM_BUFFER)) > 0 )
        write(auxFileID, buf, n);

    lseek(auxFileID, 0L, SEEK_SET);
    lseek(destFileID, 0L, SEEK_SET);

    strcpy(header.InfoF.FileName, fich_origen);
    header.InfoF.Tipo = 'Z';
    header.InfoF.Compri = 'Y';
    header.InfoF.TamOri = lseek(sourceFileID, 0L, SEEK_END);
    header.InfoF.TamComp = lseek(destFileID, 0L, SEEK_END);

    lseek(sourceFileID, 0L, SEEK_SET);
    lseek(destFileID, 0L, SEEK_SET);
    

    //insertando al final
    if (Posicion == -1){ //inserta al final

    lseek(destFileID, 0L, SEEK_END);

    write(destFileID, &header, sizeof(header));

    while ( (n = read(sourceFileID, buf, TAM_BUFFER)) > 0)
        write(destFileID, buf, n);

    } else { //insertar en Posicion. 
        for (i = 0; i < Posicion; i++){
            if ((n_aux = read(auxFileID, &header2, sizeof(header))) <=1 )
                break;
            write(destFileID, &header2, sizeof(header2));
            n_aux = read(auxFileID, buf, header2.InfoF.TamOri);
            write(destFileID, buf, n_aux);
        }

        write(destFileID, &header, sizeof(header));
        while ( (n = read(sourceFileID, buf, TAM_BUFFER)) > 0)
            write(destFileID, buf, n);

        while ((n = read(auxFileID, buf, TAM_BUFFER))>0)
            write(destFileID, buf, n);
        
    }

    close(sourceFileID);
    close(destFileID);
    close(auxFileID);
    unlink(fname);

    return 0;

}
