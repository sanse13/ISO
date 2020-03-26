#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include "util.h"

#define E_OPEN 1
#define E_OPEN_MSG "No se puede abrir file_mypackzip.\n"

#define E_CREAT_DIR 2
#define E_CREAT_DIR_MSG "No se puede crear dir_fuente.\n"

#define E_DIR 3
#define E_DIR_MSG "dir_destino no es un directorio.\n"

void getFileName(char *string1, char *string2){
    int i;
    int length = strlen(string1);
    for (i = 0; i < length; i++){
        string2[i] = string2[i+1];
        
    }
}

//debe extraer todos los ficheros contenidos en file_mypackzip perteneciente
//al directorio dir_destino

int extraer_directorio(char *dir_destino,  char *file_mypackzip)
{

    struct stat statVar;
    struct dirent *entrada;
    struct s_header header;
    int ret, n, n_aux, auxFileID;
    char ruta[512];
    char buf[TAM_BUFFER];
    char fileName[256];
    char insertar[256];
    strcpy(ruta, dir_destino);

    
    

    int sourceFileID = open(file_mypackzip, O_RDONLY);

    if (sourceFileID == -1){
        close(sourceFileID);
        write(2, E_OPEN_MSG, strlen(E_OPEN_MSG));
        return(E_OPEN);
    }

    DIR *dir;
    dir = opendir(dir_destino);

    //necesito coger la ruta

    if (dir == NULL){ //cuando no existe el directorio
        if (mkdir(dir_destino, S_IRWXU | S_IRWXG) == -1){
            write(2, E_CREAT_DIR_MSG, strlen(E_CREAT_DIR_MSG));
            return E_CREAT_DIR;
        }
    }

    ret = stat(dir_destino, &statVar);
    if (!S_ISDIR(statVar.st_mode)){
        write(2, E_DIR_MSG, strlen(E_DIR_MSG));
        return E_DIR;
    }
    
        dir = opendir(dir_destino);
        
        quitar_primero(ruta);
        //meter el bucle para sacar el header
        while ( (n = read(sourceFileID, &header, sizeof(header))) >0)
        {
            if (strncmp(ruta, header.InfoF.FileName, strlen(ruta)-1) == 0){
                
                auxFileID = open(header.InfoF.FileName, O_CREAT, 0644);
                n = read(sourceFileID, buf, header.InfoF.TamOri);
                write(auxFileID, buf, n);
            } else

            lseek(sourceFileID, header.InfoF.TamOri, SEEK_CUR);
            
        }

        close(sourceFileID);
        if (auxFileID > 2) close(auxFileID);
        closedir(dir);
    return 0;

}