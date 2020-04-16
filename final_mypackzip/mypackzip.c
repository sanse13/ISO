#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

void help(){
    printf("-e\t Extrae un fichero del fichero comprimido. \t\t<./mypackzip -e file_mypackzip Posicion>\n");
    printf("-i\t Inserta un fichero en el fichero comprimido. \t\t<./mypackzip -i fich_origen Posicion file_mypackzip>\n");
    printf("-d\t Inserta un directorio en el fichero comprimido. \t<./mypackzip -d dir_fuente comprimido>\n");
    printf("-ed\t Extrae el contenido del fichero de un comprimido. \t<./mypackzip -ed dir_destino comprimido>\n");
    printf("-p\t Cambia el modo de la terminal a no canonico. \t\t<./mypackzip -p>\n");
    printf("-pr\t Recupera el estado de la terminal al modo canonico. \t<>\n");
    printf("-c\t Eco.\n");
    printf("-f\t Funcionalidad para comprimir. \t\t\t\t<./mypackzip -f FICH_ORIGEN COMPRIMIDO>\n");
}

int main(int argc, char* argv[]){

    struct s_header header;
    struct termios tm;
    char respuesta;
    int res, ret;
    int sourceFileID, destFileID;

    if (argc == 1){
        write(2, ERR_MSG5, strlen(ERR_MSG5));
        _exit(ERR5);
    }


    char caso[50];
    strcpy(caso, argv[1]);
    if (caso[0] == '-'){

        if (strcmp(argv[1], "-help") == 0) {
            help();
            return 0;
        }

        if (strcmp(argv[1], "-e") == 0){
            //./mypackzip -e comprimido Posicion
            if (argc != 4){
                write(2, ERR_MSG1, strlen(ERR_MSG1));
                _exit(ERR1);
            }
            int pos = atoi(argv[3]);
            res = extraer_fichero(argv[2], pos);
            if (res != 0) _exit(res); else return res;
        }

        if (strcmp(argv[1], "-i") == 0){
            //./mypackzip -i origen Posicion comprimido
            if (argc != 5){
                write(2, ERR_MSG1, strlen(ERR_MSG1));
                _exit(ERR1);
            }
            int pos = atoi(argv[3]);
            res = insertar_fichero(argv[2], pos, argv[4]);
            if (res != 0) _exit(res); else return res;
        }

        if (strcmp(argv[1], "-d") == 0){
            // ./mypackzip -d origen comprimido
            if (argc != 4){
                write(2, ERR_MSG1, strlen(ERR_MSG1));
                _exit(ERR1);
            }
            res = insertar_directorio(argv[2], argv[3]);
            if (res != 0) _exit(res); else return res;
        }

        if (strcmp(argv[1], "-ed") == 0){
            // ./mypackzip -ed dir_destino comprimido
            if (argc != 4){
                write(2, ERR_MSG1, strlen(ERR_MSG1));
                _exit(ERR1);
            }
            res = extraer_directorio(argv[2], argv[3]);
            if (res != 0) _exit(res); else return res;
        }

        if (strcmp(argv[1], "-p") == 0){
            // ./mypackzip -p
            if (argc != 2){
                write(2, ERR_MSG1, strlen(ERR_MSG1));
                _exit(ERR1);
            }
            printf("Atencion, ¿esta seguro de que desea cambiar el modo de la terminal a no canonico?\n");
            printf("Pulse 'y' para continuar, 'n' para abortar.\n");
            scanf("%c", &respuesta);
            if(respuesta == 'y'){
                printf("Terminal en modo no canonico\n.");
                setTermNoCanon(&tm);
            } else {
                printf("Operacion abortada.\n");
            }
            return 0;
        }

        if (strcmp(argv[1], "-pr") == 0){
            printf("Regresando a modo canonico.\n");
            eco(tm);
            return 0;
        }

        if (strcmp(argv[1], "-c") == 0){
            printf("Eco.\n");
            eco(tm);
            return 0;
        }

        if (strcmp(argv[1], "-f") == 0){
            printf("Comprimiendo...\n");
            sourceFileID = open(argv[2], O_RDONLY);
            destFileID = open(argv[3], O_CREAT | O_RDWR, 0777);
            ret = def(sourceFileID, destFileID, Z_DEFAULT_COMPRESSION, argv[2], argv[3]);
            if (ret != Z_OK)
                zerr(ret);
            return ret;
        }
    } else {

        if (argc < 3){
            write(2, ERR_MSG1, strlen(ERR_MSG1));
            _exit(ERR1);
        }

        sourceFileID = open(argv[1], O_RDONLY);
        
        destFileID = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, 0644);

        if (sourceFileID == -1){ 
            write(2, ERR_MSG2, strlen(ERR_MSG2));
            close(sourceFileID);
            close(destFileID);
            _exit(ERR2);
        }

        if (access(argv[2], W_OK) == -1){
            write(2, ERR_MSG3, strlen(ERR_MSG3));
            close(sourceFileID);
            _exit(ERR3);
        }

        strcpy(header.InfoF.FileName, argv[1]);
        header.InfoF.Compri = 'N';
        header.InfoF.Tipo = 'Z';
        header.InfoF.TamOri = lseek(sourceFileID, 0L, SEEK_END);
        header.InfoF.TamComp = header.InfoF.TamOri; 

        lseek(sourceFileID, -0L, SEEK_SET);

        write(destFileID, &header, sizeof(header));

        int n;
        char buf[TAM_BUFFER];   

        while ( (n = read(sourceFileID, buf, TAM_BUFFER)) > 0)
            write(destFileID, buf, n);

        close(sourceFileID);
        close(destFileID);

        return 0;
    }
}