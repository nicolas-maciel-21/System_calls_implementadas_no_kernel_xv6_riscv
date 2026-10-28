#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "kernel/sysinfo.h"

struct uptime_info {
  uint64 ticks;
  uint64 seconds;
  uint64 procs;
};

extern struct proc proc[];

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.

struct utsname {
  char sysname[32];
  char nodename[32];
  char release[32];
  char version[32];
  char machine[32];
};

uint64
sys_uname(void)
{
  uint64 addr;
  struct utsname un;

  //Lê o ponteiro
  argaddr(0, &addr);

  //Preenche os campos de utsname
  safestrcpy(un.sysname, "xv6", sizeof(un.sysname));
  safestrcpy(un.nodename, "riscv-node", sizeof(un.nodename));
  safestrcpy(un.release, "1.0", sizeof(un.release));
  safestrcpy(un.version, "xv6-riscv-ext", sizeof(un.version));
  safestrcpy(un.machine, "RISC-V", sizeof(un.machine));

  if (copyout(myproc()->pagetable, myproc()->sz, addr, (char *)&un,
              sizeof(un)) < 0) {
    return -1;
  }

  return 0;
}

uint64
sys_uptime(void)
{
  uint64 addr;
  struct uptime_info info;
  struct proc *p;

  //Obtem endereço do ponteiro
  if (argaddr(0, &addr) < 0)
    return -1;

  //Obtem tempo de atividade
  acquire(&tickslock);
  info.ticks = ticks;
  release(&tickslock);
  info.seconds = info.ticks / 10;

  //Conta processos ativos
  info.procs = 0;
  for (p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if (p->state != UNUSED) {
      info.procs++;
    }
    release(&p->lock);
  }

  //Copia para espaço de usuario
  if (copyout(myproc()->pagetable, myproc()->sz, addr, (char *)&info,
              sizeof(info)) < 0)
    return -1;

  return 0;
}

uint64
sys_trace(void)
{

  int mask;

  //Lê o argumento passado pelo usuário
  if (argint(0, &mask) < 0)
    return -1;

  //Guarda a mascara do processo atual
  myproc()->mask = mask;
  return 0;
}

uint64
sys_sysinfo(void)
{

  uint64 addr; //Endereço do ponteiro passado
  struct sysinfo info;

  //Obtem o ponteiro passado para sysinfo()
  if (argaddr(0, &addr) < 0)
    return -1;

  //Preenche com os dados do kernel
  info.freemem = freemem();
  info.nproc = nproc();

  //Copia os dados para a memória do processo

  if (copyout(myproc()->pagetable, myproc()->sz, addr, (char *)&info,
              sizeof(info)) < 0)
    return -1;

  return 0;
}
