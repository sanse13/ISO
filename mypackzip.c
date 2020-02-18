#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include "util.h"

int main(int argc, char* argv[]){

    struct s_header header;

    if (argc < 3){
        write(2, ERR_MSG1, strlen(ERR_MSG1));
        _exit(ERR1);
    }

    int sourceFileID = open(argv[1], O_RDONLY);
    
    int destFileID = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, 0644);

    if (sourceFileID == -1){ 
        write(2, ERR_MSG2, strlen(ERR_MSG2));
        close(sourceFileID);
        close(destFileID);
        return(ERR2);
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