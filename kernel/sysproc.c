#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64_t get_freemem(void);
uint64_t get_nproc(void);

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


// this function used to trace the system call based on bits in mask 
// if the bit is 1 then the system call is traced
uint64
sys_trace(void)
{
  int trace_mask=0;
  // get trace_mask from trap frame
  argint(0, &trace_mask);
  struct proc *p = myproc(); // get current process
  p->trace_mask = trace_mask;     // set mask for current process
  return 0;
}

uint64 sys_sysinfo(void) {
    struct sysinfo info;
    info.freemem = get_freemem();
    info.nproc = get_nproc();

    uint64 addr;
    argaddr(0, &addr); // Lấy địa chỉ struct từ user space
    // Kiểm tra địa chỉ trước khi copyout
    if (addr == 0 || addr >= myproc()->sz) {
        return -1;
    }

    if (copyout(myproc()->pagetable, addr, (char *)&info, sizeof(info)) < 0)
        return -1;

    return 0;
}
