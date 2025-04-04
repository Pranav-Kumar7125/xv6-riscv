#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"
#include "syscall_stats.h"
#include "syscall_report.h" // Include the new header

// Define the actual instance
struct syscall_stats syscall_stats;

void init_syscall_stats()
{
  initlock(&syscall_stats.lock, "syscall_stats");
  syscall_stats.call_count = 0;
  syscall_stats.last_minute = 0;
  syscall_stats.current_hour = 0;
  syscall_stats.calls_this_hour = 0;
  syscall_stats.hours_tracked = 0;
  syscall_stats.startup_time = 0;
  for (int i = 0; i < 60; i++)
  {
    syscall_stats.calls_per_minute[i] = 0;
  }
}

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
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
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
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

uint64
sys_23052801(void)
{
  uint64 addr;

  // Correct way to use argaddr()
  argaddr(0, &addr); // This just retrieves the address, doesn't return success/failure

  struct syscall_report
  {
    int calls_per_minute[60];
    int current_minute;
    int hourly_average;
    int hours_tracked;
  } report;

  acquire(&syscall_stats.lock);

  // Copy per-minute data
  for (int i = 0; i < 60; i++)
  {
    report.calls_per_minute[i] = syscall_stats.calls_per_minute[i];
  }

  report.current_minute = syscall_stats.last_minute % 60;

  // Calculate hourly average
  if (syscall_stats.hours_tracked > 0)
  {
    report.hourly_average = syscall_stats.calls_this_hour / syscall_stats.hours_tracked;
  }
  else
  {
    report.hourly_average = 0;
  }

  report.hours_tracked = syscall_stats.hours_tracked;

  release(&syscall_stats.lock);

  // Copy to user space - this is where we check for errors
  if (copyout(myproc()->pagetable, addr, (char *)&report, sizeof(report)) < 0)
  {
    return -1;
  }

  return 0;
}