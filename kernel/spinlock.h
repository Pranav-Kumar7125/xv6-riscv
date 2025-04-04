#ifndef _SPINLOCK_H
#define _SPINLOCK_H

struct spinlock
{
  uint locked;
  char *name;
  struct cpu *cpu;
};

// Function declarations...
void acquire(struct spinlock *);
void release(struct spinlock *);
void initlock(struct spinlock *, char *);

#endif // _SPINLOCK_H