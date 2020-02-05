#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFF 512

#define ERR2 "Falta el argumento del mensaje. Pruebe de nuevo.\n"
#define ERR3 "Error de argumentos. El uso es ./despertador [SEGUNDOS] [MENSAJE1] [MENSAJE2] ...\n"


int main(int argc, char* argv[]){

	
	if (argc == 2){

                write(1, ERR2, strlen(ERR2));
                _exit(1);

        }


	if (argc < 3){

		write(1, ERR3, strlen(ERR3));
		_exit(1);

	}

	int segundos = atoi(argv[1]);

	int segundos_tr = 0;

	if (segundos < 0){

		int i = 2;
		int msgs = argc - 2;
		while (msgs != 0){

			write(1, argv[i], strlen(argv[i]));
			write(1, "\n", sizeof("\n"));
			msgs--;
			i++;
		}

		_exit(1);

	}

	char *concat_str;
	concat_str = (char *)malloc(MAX_BUFF);


	while (segundos != -1){
		
		//printf("Han pasado %d segundos y quedan %d segundos.\n", segundos_tr, segundos);
		sprintf(concat_str, "Han pasado %d segundos y quedan %d segundos\n", segundos_tr, segundos);
		write(1, concat_str, strlen(concat_str));
		sleep(1);
		segundos--;
		segundos_tr++; 

	}
	
	int msgs = argc-2;
	int i = 2;
	while (msgs != 0){

		write(1, argv[i], strlen(argv[i]));
		write(1, "\n", strlen("\n"));
		i++;
		msgs--;
	
	}

	_exit(0);
} 