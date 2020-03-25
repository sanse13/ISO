#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#include "util.h"

#define E_OPEN 1
#define E_OPEN_MSG "No se puede abrir o crear file_mypackzip.\n"

#define E_OPEN2 2
#define E_OPEN2_MSG "No se puede abrir dir_fuente.\n"

#define E_DIRECTORIO 3
#define E_DIRECTORIO_MSG "Este fichero no es un directorio.\n"

//inserta todo el contenido del dir_fuente al final del archivo file_mypackzip
//debe insertar cada una de las entradas de tipo fichero regular del dir_fuente
//el nombre de cada fichero regular debe contener la ruta relativa de dir_fuente
int insertar_directorio(char *dir_fuente,  char *file_mypackzip){

    struct stat statVar;
    char fichero[TAM_BUFFER];
    char actualpath[_PC_PATH_MAX+1];
    int ret;

    int destFileID = open(file_mypackzip, O_CREAT | O_RDWR, 0777);

    if (destFileID == -1){
        write(2, E_OPEN_MSG, strlen(E_OPEN_MSG));
        return(E_OPEN);
    }

    char cwd[1024];
    //chdir(dir_fuente);
    getcwd(cwd, sizeof(cwd));
    char directorio3[512];
 //   strcpy(directorio, getcwd(cwd, sizeof(cwd)));

    
    char linea[512];
    
    DIR *dir;
    struct dirent *entrada;
    struct s_header header;

    ret = stat(dir_fuente, &statVar);

    if (!S_ISDIR(statVar.st_mode)){
        close(destFileID);
        write(2, E_DIRECTORIO_MSG, strlen(E_DIRECTORIO_MSG));
        return(E_DIRECTORIO);
    }

    int tamComp = lseek(destFileID, 0L, SEEK_END);
    lseek(destFileID, 0L, SEEK_SET);

    dir = opendir(dir_fuente);

    if (dir == NULL){
        closedir(dir);
        close(destFileID);
        write(2, E_OPEN2_MSG, strlen(E_OPEN2_MSG));
        return(E_OPEN2);
    }

    chdir(dir_fuente);
    quitar_primero(dir_fuente);
    if (strcmp(&dir_fuente[strlen(dir_fuente)-1], "/") == 0){ 
        strcpy(directorio3, "");
        strncat(directorio3, dir_fuente, strlen(dir_fuente)-1);
        strcpy(dir_fuente, directorio3);
    }
    
    while ((entrada = readdir(dir)) != NULL){
        stat(entrada->d_name, &statVar);
        if (S_ISREG(statVar.st_mode)){
            strcpy(header.InfoF.FileName, dir_fuente);
            strcat(header.InfoF.FileName, "/");
            strcat(header.InfoF.FileName, entrada->d_name);
            header.InfoF.Tipo = 'Z';
            header.InfoF.Compri = 'Y';
            header.InfoF.TamOri = statVar.st_size;
            header.InfoF.TamComp = tamComp;
            write(destFileID, &header, sizeof(header));
        }

    }

    closedir(dir);
    close(destFileID);
    return 0;

}
