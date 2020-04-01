/* time_checker.c */

/* ./time_checker  nb_tests  min_time  max_time  program [parameters]  */
/* probar si un programa se ejecuta o no en un intervalo de tiempo determinado */

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  int nb_tests = atoi(argv[1]);
  int min_time = atoi(argv[2]);
  int max_time = atoi(argv[3]);
  int clockid_min, childid, aux, id, tiempo_de_menos, t, aux_nb_tests, t_total;
  aux_nb_tests = nb_tests;

  t_total = time(0);
  if (aux_nb_tests == 0){
    printf("Debe de ser mayor que cero el numero de pruebas\n");
    return 0;
  }

  while (aux_nb_tests > 0){
    /* se realizara el numero de pruebas indicado*/
    printf("Probando para %d segundos\n", min_time);
    if ( (clockid_min = fork()) == 0)
      sleep(min_time);
    if ( (childid = fork()) == 0)
      execvp(argv[4], &(argv[4]));
    t = time(0);
    id = wait(NULL);
    if (id == childid) {   /* child finishes first; stop clock */
       kill(clockid_min, SIGKILL);
       wait(NULL);
       aux = 0;
    } else {   /* clock finishes first; force child to finish */
       kill(childid, SIGKILL);
       wait(NULL);
       printf("--overtime\n");
       aux = -1;
   }

   t = time(0) - t;
   printf("--child time: %d\n", t);
   printf("El programa ha sido %d/%d veces mas rapido de %d segundos\n\n\n", t, min_time, min_time);

   printf("Probando para %d segundos\n", max_time);
   if ( (clockid_min = fork()) == 0)
     sleep(max_time);
   if ( (childid = fork()) == 0)
     execvp(argv[4], &(argv[4]));
     t = time(0);
     id = wait(NULL);
     if (id == childid) {   /* child finishes first; stop clock */
        kill(clockid_min, SIGKILL);
        wait(NULL);
        aux = 0;
     } else {   /* clock finishes first; force child to finish */
        kill(childid, SIGKILL);
        wait(NULL);
        printf("--overtime\n");
        aux = -1;
    }

    t = time(0) - t;
    printf("--child time: %d\n", t);
    printf("El programa ha sido %d/%d mas veces lento de %d segundos\n\n\n", t, max_time, max_time);

    /* y ahora el tiempo entre min y max */
    float entre = max_time/min_time;
    aux_nb_tests--;
  }

  t_total = time(0) - t_total;
  printf("El tiempo total ha sido %d segundos\n", t_total);
  return 0;
}
