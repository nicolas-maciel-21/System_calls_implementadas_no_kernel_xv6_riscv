#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/sysinfo.h"

int main(int argc, char *argv[]) {
  printf("\n========================================\n");
  printf("   INICIANDO BATERIA DE TESTES - XV6    \n");
  printf("========================================\n\n");

  // 1. Teste da System Call: uptime
  {
    struct uptime_info up;
    printf("[TESTE 1] Testando uptime()...\n");
    if (uptime(&up) == 0) {
      printf("  [SUCESSO] Ticks: %d | Segundos: %d | Processos: %d\n", 
             (int)up.ticks, (int)up.seconds, (int)up.procs);
    } else {
      printf("  [FALHA] uptime() retornou erro.\n");
    }
  }

  // 2. Teste da System Call: sysinfo e Variação Dinâmica de Memória
  {
    struct sysinfo info_inicial, info_pos_aloc;
    printf("\n[TESTE 2] Testando sysinfo() e variacao de memoria...\n");
    
    if (sysinfo(&info_inicial) == 0) {
      printf("  [INFO] Memoria livre inicial: %d bytes\n", (int)info_inicial.freemem);
      printf("  [INFO] Processos ativos iniciais: %d\n", (int)info_inicial.nproc);
    } else {
      printf("  [FALHA] sysinfo() inicial falhou.\n");
    }

    // Alocar 40 KB de memória com sbrk() e utilizá-la para forçar a alocação física
    printf("  [AÇÃO] Alocando 40.960 bytes via sbrk()...\n");
    char *ptr = sbrk(40 * 1024);
    if (ptr != (char *)-1) {
      ptr[0] = 0; // Utiliza a variável para evitar erro de variável não usada e forçar page fault/alocação
    }
    
    if (sysinfo(&info_pos_aloc) == 0) {
      printf("  [INFO] Memoria livre apos alocacao: %d bytes\n", (int)info_pos_aloc.freemem);
      if (info_pos_aloc.freemem < info_inicial.freemem) {
        printf("  [SUCESSO] Variação dinâmica validada! A memória livre diminuiu corretamente.\n");
      } else {
        printf("  [AVISO] A memória livre não diminuiu como esperado.\n");
      }
    }
    
    // Libertar a memória alocada
    sbrk(-40 * 1024);
  }

  // 3. Teste de Robustez: Ponteiros Inválidos (Proteção do Kernel / copyout)
  {
    printf("\n[TESTE 3] Testando robustez com ponteiros invalidos...\n");
    
    // Teste com ponteiro NULL (0x0)
    if (sysinfo((struct sysinfo *)0x0) == -1) {
      printf("  [SUCESSO] sysinfo() rejeitou ponteiro NULL (0x0) com segurança (retornou -1).\n");
    } else {
      printf("  [FALHA] sysinfo() aceitou ponteiro NULL incorretamente.\n");
    }

    // Teste com endereço de memória inválido/fora do espaço do processo
    if (sysinfo((struct sysinfo *)0xffffffffffffffff) == -1) {
      printf("  [SUCESSO] sysinfo() rejeitou endereço de memória inválido com segurança (retornou -1).\n");
    } else {
      printf("  [FALHA] sysinfo() aceitou endereço inválido incorretamente.\n");
    }
  }

  // 4. Teste da System Call: trace
  {
    printf("\n[TESTE 4] Testando trace()...\n");
    if (trace(32) == 0) {
      printf("  [SUCESSO] trace(32) ativado com sucesso.\n");
    } else {
      printf("  [FALHA] trace() retornou erro.\n");
    }
  }

  printf("\n========================================\n");
  printf("   TODOS OS TESTES FORAM EXECUTADOS     \n");
  printf("========================================\n\n");
  
  exit(0);
}
