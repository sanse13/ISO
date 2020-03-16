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
    int ret, n, auxFileID;
    char ruta[512];
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
        mkdir(dir_destino, S_IRWXU | S_IRWXG);
        dir = opendir(dir_destino);
        
        quitar_primero(ruta);
        
        //meter el bucle para sacar el header
        while ( (n = read(sourceFileID, &header, sizeof(header))) >0)
        {
            
            if (strncmp(ruta, header.InfoF.FileName, strlen(ruta)-1) == 0){
                printf("FileName: %s\n", header.InfoF.FileName);
                strcpy(insertar, "/");
                strcat(insertar, header.InfoF.FileName);
                printf("insertar es: %s\n", insertar);
                auxFileID = open(insertar, O_CREAT, 0777);
            }
            
            
            lseek(sourceFileID, header.InfoF.TamComp, SEEK_CUR);
        }

        //meter en el directorio todos los ficheros relacionados con la ruta

        close(sourceFileID);
        closedir(dir);
    } else {        //cuando existe el directorio
        ret = stat(dir_destino, &statVar);
        strcpy(ruta, dir_destino);
        quitar_primero(ruta);

        while ( (n = read(sourceFileID, &header, sizeof(header))) >0)
        {
            
            if (strncmp(ruta, header.InfoF.FileName, strlen(ruta)-1) == 0){
                printf("FileName: %s\n", header.InfoF.FileName);
                strcpy(insertar, "/");
                strcat(insertar, header.InfoF.FileName);
                printf("insertar es: %s\n", insertar);
                auxFileID = open(insertar, O_CREAT, 0777);
            }
            
            
            lseek(sourceFileID, header.InfoF.TamComp, SEEK_CUR);
        }


        closedir(dir);
        close(sourceFileID);
    }
    return 0;

}