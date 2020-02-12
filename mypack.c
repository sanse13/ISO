#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#define RESTO_HEADER (TAM_S_HEADER - TAM_S_INFO)
#define TAM_S_HEADER    512
#define MAX_FILE_NAME   256
#define TAM_BUFFER      512
#define TAM_S_INFO sizeof(struct s_info )

#define ERR1 1
#define ERR2 2
#define ERR3 3
#define ERR4 4

#define MSG1 "Error en los argumenots\n"
#define MSG2 "No existe el fichero de origen\n"
#define MSG3 "No se dispone de permisos para esta operación. No se dispone de permisos para crear o modificar fich_destino\n"
#define MSG4 "Otro tipo de errores\n"

struct s_info {
           char Tipo;                       // ='Z'
           char Compri;                     // ='N' (por defecto) o 'Y'. Indica si los datos del  
                                            // fichero están almacenados de forma comprimida
           unsigned long TamOri;            // Tamanio original del fichero (sin comprimir)
           unsigned long TamComp;           // Tamanio del fichero comprimido. Si los datos no estan
                                            // comprimidos, su valor es el mismo de TamOri-
           char FileName[MAX_FILE_NAME];    // Nombre del fichero origen 
}sInfo;

struct  s_header {
           struct s_info InfoF;                 // Información del fichero
           char RestoHeader[RESTO_HEADER];  // ToDo...  
}sHeader;


int main(int argc, char* argv[]){

    if (argc < 3){
        write(2, MSG1, strlen(MSG1));
        _exit(ERR1);
    }

    int sourceFileID = open(argv[1], O_CREAT | O_RDONLY);
    if (sourceFileID == -1){
        write(2, MSG2, strlen(MSG2));
        _exit(ERR2);
    }

    strcpy(sInfo.FileName, argv[1]);
    if (sourceFileID == -2){
        write(2, MSG2, strlen(MSG2));
        _exit(ERR3);
    }

    //otros errores (ERR4)

    int destFileID = open(argv[2], O_CREAT | O_RDWR | O_APPEND, 0644);
    int n;
    char buf[TAM_BUFFER];
    while ((n = read(sourceFileID, buf, TAM_BUFFER)) > 0){
        write(destFileID, buf, n);
    }

    close(sourceFileID);
    close(destFileID);

    return 0;
}