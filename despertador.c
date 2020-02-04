#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	
	if (argc == 2){

                printf("Falta el argumento del mensaje. Pruebe de nuevo.\n");
                exit(2);

        }


	if (argc < 3){
		printf("Error de argumentos. El uso es ./despertador [SEGUNDOS] [MENSAJE1] [MENSAJE2] ...\n");
		exit(1);
	}

	int segundos = atoi(argv[1]);

	int segundos_tr = 0;

	if (segundos < 0){

		int i = 2;
		int msgs = argc - 2;
		while (msgs != 0){

			printf("%s\n", argv[i]);
			msgs--;
			i++;
		}

		exit(1);

	}

	while (segundos != -1){
		
		printf("Han pasado %d segundos y quedan %d segundos.\n", segundos_tr, segundos);
		sleep(1);
		segundos--;
		segundos_tr++; 

	}
	
	int msgs = argc-2;
	int i = 2;
	while (msgs != 0){

		printf("%s\n", argv[i]);
		i++;
		msgs--;
	
	}

	exit(0);
} 
