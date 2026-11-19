# Trabalho de Sistemas Operacionais — Extensão do Kernel xv6-RISC-V


## 1. Arquivos Modificados e Criados

### Kernel
* `kernel/syscall.h`: Definição dos números das novas chamadas de sistema (`SYS_trace`, `SYS_uname`, `SYS_uptime`, `SYS_sysinfo`).
* `kernel/syscall.c`: Tabela de despacho (`syscalls[]`), nomes para rastreamento e lógica de interceptação do `trace`.
* `kernel/sysproc.c`: Implementação das funções `sys_trace`, `sys_uname`, `sys_uptime` e `sys_sysinfo`.
* `kernel/proc.h` e `kernel/proc.c`: Adição da máscara de rastreamento (`tracemask`) na `struct proc`, herança via `fork()` e função de contagem de processos ativos.
* `kernel/kalloc.c`: Função auxiliar para contabilizar a quantidade real de memória física livre (páginas livres na `kmem.freelist`).
* `kernel/defs.h`: Protótipos das novas funções internas do kernel.
* `kernel/sysinfo.h` (e/ou `kernel/uname.h`): Definição das estruturas de dados trocadas entre kernel e espaço de usuário.

### Espaço de Usuário (User Space)
* `user/user.h`: Declarações das assinaturas de funções e estruturas para programas de usuário.
* `user/usys.pl`: Geração dos *stubs* em assembly RISC-V (`ecall`) para as quatro chamadas.
* `user/trace.c`, `user/uname.c`, `user/uptime.c`, `user/sysinfo.c`: Programas utilitários de linha de comando.
* `Makefile`: Inclusão dos novos programas na variável `UPROGS`.

---

## 2. Interfaces Implementadas
1. **`int trace(int mask)`**: Ativa o rastreamento de *syscalls* cujos bits correspondentes estejam ativos em `mask`. Retorna `0` em caso de sucesso.
2. **`int uname(struct utsname *buf)`**: Copia informações de identificação do sistema (sistema, nó, release, versão, máquina) para o ponteiro `buf` no espaço de usuário usando `copyout`. Retorna `0` ou `-1` se o endereço for inválido.
3. **`int uptime(struct uptime_info *info)`** *(ou retorno direto em ticks)*: Retorna o tempo de atividade do sistema desde a inicialização. Retorna `-1` em caso de ponteiro inválido.
4. **`int sysinfo(struct sysinfo *info)`**: Coleta em tempo real a memória livre em bytes (`freemem`) e o número de processos não-`UNUSED` (`nproc`), copiando via `copyout`. Retorna `0` ou `-1` se o endereço for inválido.

---

## 3. Versões das Ferramentas e Execução
* **SO Ambiente:** Ubuntu/WSL2
* **Compilador:** `riscv64-linux-gnu-gcc` (versão 15.2.0)
* **Emulador:** `qemu-system-riscv64` (versão 10.2.1)
