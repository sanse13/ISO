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
#define E_OPEN_MSG "No se puede abrir o crear file_mypackzip.\n"

#define E_OPEN2 2
#define E_OPEN2_MSG "No se puede abrir dir_fuente.\n"

#define E_DIRECTORIO 3
#define E_DIRECTORIO_MSG "Este fichero no es un directorio.\n"

 void quitar_primero(char *cadena){
    int i;
    int size = strlen(cadena);
    for (i = 0; i < size; i++){
        cadena[i] = cadena[i+1];
    }
 }

//inserta todo el contenido del dir_fuente al final del archivo file_mypackzip
//debe insertar cada una de las entradas de tipo fichero regular del dir_fuente
//el nombre de cada fichero regular debe contener la ruta relativa de dir_fuente
int insertar_directorio(char *dir_fuente,  char *file_mypackzip){

    struct stat statVar;
    char fichero[TAM_BUFFER];
    char actualpath[_PC_PATH_MAX+1];
    int ret;

    int destFileID = open(file_mypackzip, O_CREAT | O_WRONLY, 0777);

    char cwd[1024];
    chdir(dir_fuente);
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);
    //tiene que poner iso git/ISO
    char directorio[512];
    strcpy(directorio, getcwd(cwd, sizeof(cwd)));
    quitar_primero(directorio);
    printf("%s\n", directorio);
    char linea[512];
    

    DIR *dir;
    struct dirent *entrada;

    ret = stat(dir_fuente, &statVar);

    //corregir errores del open del destFileID
    lseek(destFileID, SEEK_END, 0L);

    if (!S_ISDIR(statVar.st_mode)){
        write(2, E_DIRECTORIO_MSG, strlen(E_DIRECTORIO_MSG));
        return E_DIRECTORIO;
    }

    dir = opendir(dir_fuente);

    while ((entrada = readdir(dir)) != NULL){
        stat(entrada->d_name, &statVar);
        //incluir solo los regulares
        if (S_ISREG(statVar.st_mode)){
            sprintf(linea, "%s %s\n", directorio, entrada->d_name);
            write(destFileID, linea, strlen(linea));    
        }
    }

    closedir(dir);
    close(destFileID);
    return 0;

}