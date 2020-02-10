#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_BUFF 256

int main(int argc, char* argv[]){
    
    if (argc < 2){
        printf("Uso incorrecto. Uso: ./ctee [FICHERO]\n");
        exit(1);
    }

    FILE * fp;
    fp = fopen(argv[1], "w+");

    char buf[TAM_BUFF];

    //EOF ctrl+D
    while (scanf("%s", buf) != EOF){
        printf("%s\n", buf);
        fprintf(fp, "%s\n", buf);
        }

    fclose(fp);
    return(0);
}