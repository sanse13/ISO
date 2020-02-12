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

#define MSG1 "Error en los argumentos\n"
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
};

struct  s_header {
           struct s_info *InfoF;                 // Información del fichero
           char RestoHeader[RESTO_HEADER];       // ToDo...  
};


int main(int argc, char* argv[]){
    struct s_header *s_h = malloc(TAM_S_HEADER);
    s_h->InfoF = malloc(TAM_S_INFO);

    if (argc < 3){
        write(2, MSG1, strlen(MSG1));
        _exit(ERR1);
    }

    int sourceFileID = open(argv[1], O_RDONLY);
    int destFileID = open(argv[2], O_CREAT | O_RDWR, 0644);

    if (sourceFileID == -1){
        write(2, MSG2, strlen(MSG2));
        close(sourceFileID);
        close(destFileID);
        _exit(ERR2);
    }

    if (access(argv[2], F_OK | W_OK) == -1){
        write(2, MSG3, strlen(MSG3));
        close(sourceFileID);
        close(destFileID);
        _exit(ERR3);
    }

    //otros errores (ERR4)
    //¿header? campo a campo o de golpe? 
    //error 4 -> ________???

    strcpy(s_h->InfoF->FileName, argv[1]);
    s_h->InfoF->Compri = 'N';
    s_h->InfoF->Tipo = 'Z';
    s_h->InfoF->TamOri = lseek(sourceFileID, 0L, SEEK_END);
    s_h->InfoF->TamComp = s_h->InfoF->TamOri;

    lseek(sourceFileID, -0L, SEEK_SET);

    int tam = (int)s_h->InfoF->TamOri;
    char str[TAM_BUFFER];
    sprintf(str, "%d", tam);



    write(destFileID, "Tipo del comprimido: ", strlen("Tipo del comprimido: "));
    write(destFileID, &s_h->InfoF->Compri, 1);
    write(destFileID, "\n", 1);

    write(destFileID, "Nombre del archivo: ", strlen("Nombre del archivo: "));
    write(destFileID, s_h->InfoF->FileName, strlen(s_h->InfoF->FileName));
    write(destFileID, "\n", 1);

    write(destFileID, "Tipo: ", strlen("Tipo: "));
    write(destFileID, &s_h->InfoF->Tipo, 1); 
    write(destFileID, "\n", 1);

    write(destFileID, "Tamaño del fichero origen: ", strlen("Tamaño del fichero origen: "));
    write(destFileID, str, strlen(str));
    write(destFileID, "\n", 1);

    write(destFileID, "Tamaño del fichero comprimido: ", strlen("Tamaño del fichero comprimido: "));
    write(destFileID, str, strlen(str));
    write(destFileID, "\n", 1);

    write(destFileID, s_h->RestoHeader, strlen(s_h->RestoHeader));
    write(destFileID, "\n", strlen("\n"));

    int n;
    char buf[TAM_BUFFER];   

    if ( (n = read(sourceFileID, buf, TAM_BUFFER)) > 0)
        write(destFileID, buf, n);

    close(sourceFileID);
    close(destFileID);

    return 0;
}