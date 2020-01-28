#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

        if (argc < 2){
                printf("Error de argumentos.\n");
                exit(1);
        }

        int segundos = atoi(argv[1]);
        int segundos_tr = 0;

        while (segundos != -1){

                printf("Han pasado %d segundos y quedan %d segundos.\n", segundos_tr, segundos);
                sleep(1);
                segundos--;
                segundos_tr++;

        }

        int msgs = argc-2;
        int i = 2;
        //printf("Hay %d mensajes\n", msgs);
        while (msgs != 0){

                printf("%s\n", argv[i]);
                i++;
                msgs--;

        }

        exit(0);
}
