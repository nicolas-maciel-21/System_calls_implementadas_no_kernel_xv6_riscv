#include "kernel/types.h"
#include "kernel/riscv.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  struct sysinfo info;

  if (sysinfo(&info) < 0) {
    fprintf(2, "%s: falhou ao obter sysinfo\n", argv[0]);
    exit(1);
  }

  printf("=== Informacoes do Sistema ===\n");
  printf("Memoria livre: %d bytes (%d Kb)\n", (int)info.freemem,
         (int)info.freemem / 1024);
  printf("Processos ativos: %d\n", (int)info.nproc);

  exit(0);
}
