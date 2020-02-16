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

#define E_DESCO "Otro tipo de errores\n"
#define ERR_DESCO 3

int extraer_fichero(char * file_mypackzip, unsigned long Posicion){
    int sourceFileID, destFileID, actual_reg, n;
    struct s_header header;
    char buf[TAM_BUFFER];

    if ( (sourceFileID = open(file_mypackzip, O_RDONLY)) == -1 ){
        write(2, E_OPEN, strlen(E_OPEN));
        _exit(ERR_OPEN);
    }

    for (actual_reg = 0; actual_reg < Posicion; actual_reg++)
    {
        if ( (n = read(sourceFileID, &header, sizeof(header)) ) <= 0 )
        {
            write(2, E_POS, strlen(E_POS));
            _exit(ERR_POS);
        }

        read(sourceFileID, buf, sizeof(header));
    }

    read(sourceFileID, &header, sizeof(header));

    printf("FileName: %s\n", header.InfoF.FileName);
    printf("Tipo: %c\n", header.InfoF.Tipo);

    //crear fichero con el contenido del fichero extraido

    close(sourceFileID);
    
    return 0;

}
