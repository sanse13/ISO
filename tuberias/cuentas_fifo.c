/* Jon Villanueva y Adrian San Segundo */
/* este programa es equivalente a who | grep acaf00 | wc -l */ 
/* ejercicio 2 de Tuberías */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define error(a) {perror(a); exit(1);};

/* filtrar en who los que son de acaf00xx */
/* implementar who | grep acaf00 */
/* el primer hijo le va a pasar el segundo el who y el segundo va a hacer grep del who */

main(){
    int pid;
    if (mkfifo("my_fifo", 0644) == -1) error("mkfifo");
    if (mkfifo("fifo2", 0644) == -1) error("fifo2");

    switch (pid = fork()) {
        case -1: error("fork");
        case 0: /* 1st child process: who */
            if (close(1) == -1) error("close");
            if (open("my_fifo", O_WRONLY) != 1) error("open");
              execlp("who", "who", NULL);
              error("execlp");
        break;
    }

    switch (pid = fork()){
        case -1: error("fork");
            break;
        case 0:
            if (close(0) == -1) error("close");
            if (open("my_fifo", O_RDONLY) != 0) error("open");
            if (close(1) == -1) error("close");
            if (open("fifo2", O_WRONLY) != 1) error("open");
                execlp("grep", "grep", "acaf00", NULL);
                error("execlp");
        break;
    }

    switch (pid = fork()){
        case -1: error("fork");
            break;
        case 0:
            if (close(0) == -1) error("close");
            if (open("fifo2", O_RDONLY) != 0) error("open");
                execlp("wc", "wc", "-l", NULL);
                error("execlp");
            break;
    
    }


    while ((pid = wait(NULL)) != -1)

    unlink("my_fifo");
    unlink("fifo2");
    printf("fifos removed\n");

    exit(0);


}