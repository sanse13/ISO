#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void quitar_primero (char* dir_fuente){
    int i;
    char c = dir_fuente[0];
    int length = strlen(dir_fuente);
    if (c == '/'){
        for (i = 0; i < length; i++){
            dir_fuente[i] = dir_fuente[i+1];
        }
    }
}