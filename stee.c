#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

#define TAM_BUFF 256

int main(int argc, char* argv[]){
    int sourceFile = open(argv[1], O_CREAT | O_WRONLY, 0644);

    char buf[TAM_BUFF];

    int n;
    while ((n = read(0, buf, TAM_BUFF)) != 0){
        write(1, buf, n);
        write(sourceFile, buf, n);
    }

    close(sourceFile);
    return 0;
}