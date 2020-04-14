#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "util.h"

#define E_OPEN "No se puede abrir file_mypackzip\n"
#define ERR_OPEN 1

#define E_POS "Número de registro erróneo (Posicion)\n"
#define ERR_POS 2

#define E_DESCO "Otro tipo de errores\n"
#define ERR_DESCO 3

int extraer_fichero(char * file_mypackzip, unsigned long Posicion)
{
    int sourceFileID, destFileID, actual_reg, n;
    struct s_header header;
    struct stat statVar;
    char buf[TAM_BUFFER];

    if ( (sourceFileID = open(file_mypackzip, O_RDONLY)) == -1 )
    {
        write(2, E_OPEN, strlen(E_OPEN));
        return ERR_OPEN;
    }

    for (actual_reg = 0; actual_reg < Posicion; actual_reg++)
    {
        if ( (n = read(sourceFileID, &header, sizeof(header)) ) <= 0 )
        {
            write(2, E_POS, strlen(E_POS));
            return ERR_POS;
        }

        lseek(sourceFileID, header.InfoF.TamOri, SEEK_CUR);
    }

    read(sourceFileID, &header, sizeof(header));

    printf("FileName: %s\n", header.InfoF.FileName);

    lstat(header.InfoF.FileName, &statVar);

    if (header.InfoF.Tipo == 'S'){
        symlink(header.InfoF.OriginalName, header.InfoF.FileName);
        close(sourceFileID);
    } else {

    destFileID = open(header.InfoF.FileName, O_CREAT | O_WRONLY, 0644);
    n = read(sourceFileID, buf, header.InfoF.TamOri);
    write(destFileID, buf, n);

    close(sourceFileID);
    close(destFileID);
    }
    
    return 0;

}
