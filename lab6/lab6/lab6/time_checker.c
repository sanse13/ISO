/* time_checker.c */

/* Aitor Urkiza, Jon Villanueva, Adrián San Segundo */

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[])
{
   int childid, clockid, clockid2, id, aux;
   long t, total;
   int loops = atoi(argv[1]);
   int menos = 0;
   int mas = 0;
   int ok = 0;
   int minimo = atoi(argv[2]);
   int maximo = atoi(argv[3]);

   total = time(0);

   while(loops > 0){
      
      if ((clockid = fork()) == 0) execlp("despertador", "despertador", argv[3], NULL);
      if ((childid = fork()) == 0) execvp(argv[4], &(argv[4]));


      t = time(0);
      id = wait(NULL);
      if (id == childid) {   /* child finishes first; stop clock */
         kill(clockid, SIGKILL);
         wait(NULL);
         aux = 0;
      } else {   /* clock finishes first; force child to finish */
         kill(childid, SIGKILL);
         wait(NULL);
         mas++;
         aux = -1;
      }
      t = time(0) - t;
      if (t < minimo) menos++;
      if (t > maximo) mas++;
      if (t > minimo && t < maximo) ok++;
      loops--;
   }

  total = time(0) - total;
  printf("too fast: %d\n", menos);
  printf("too slow: %d\n", mas);
  printf("ok: %d\n", ok);
  printf("total: %d\n", total);
  exit(aux);
}
