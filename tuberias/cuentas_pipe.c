/* Jon Villanueva y Adrian San Segundo */
/* el comando equivalente seria cat /ect/passwd | grep acaf00 | wc -l */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define error(a) {perror(a); exit(1);};

main(){
    int pfd[2], pid;
    int desc[2];

    if (pipe(pfd) == -1) error("pipe");
    if (pipe(desc) == -1) error("pipe");

    switch (pid = fork()){
        case -1: error("fork")
        case 0: /* first child cat /etc/passwd escribe el resultado en el pipe */
            if (close(1) == -1) error("close");
            if (dup(pfd[1]) != 1) error("dup");
            close(pfd[0]); close(pfd[1]);
            close(desc[0]); close(desc[1]);
            execlp("cat", "cat", "/etc/passwd", NULL);
            error ("execlp"); 
    }

    switch (pid = fork()){
        case -1: error("fork");
        case 0: /* second child grep acaf00 filtra lo que lee del pipe y lo escribe en otro pipe*/
            if (close(0) == -1) error("close");
            if (close(1) == -1) error("close");
            if (dup(pfd[0]) != 0) error("dup");
            if (dup(desc[1]) != 1) error("dup");
            close(pfd[0]); close(pfd[1]);
            close(desc[0]); close(desc[1]);
            execlp("grep", "grep", "acaf00", NULL);
            error("execlp");
    }

    switch(pid = fork()){
        case -1: error("fork");
        case 0: /* third child wc -l lee de lo que ha escrito child 2 */
            if (close(0) == -1) error("close");
            if (dup(desc[0]) != 0) error("dup");
            close(pfd[0]); close(pfd[1]);
            close(desc[0]); close(desc[1]);
            execlp("wc", "wc", "-l", NULL);
            error("execlp");
    }

    close(pfd[0]); close(pfd[1]);
    close(desc[0]); close(desc[1]);

    while ((pid = wait(NULL)) != -1)

    exit(0);

}