#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  struct uptime_info info;

  if (uptime(&info) < 0) {
    fprintf(2, "uptime: erro ao obter informacoes do sistema\n");
    exit(1);
  }

  fprintf(1, "=== Informacoes do Sistema ===\n");
  fprintf(1, "Tempo Ativo  :  %d segundos (%d ticks)\n", (int)info.seconds,
          (int)info.ticks);
  fprintf(1, "Processos  :  %d processos ativos\n", (int)info.procs);

  exit(0);
}
