#ifndef _SYSCALL_STATS_H
#define _SYSCALL_STATS_H

#include "types.h"
#include "spinlock.h"

// Define the structure type
struct syscall_stats
{
    struct spinlock lock;
    int call_count;
    uint64 last_minute;
    int calls_per_minute[60];
    int current_hour;
    int calls_this_hour;
    int hours_tracked;
    uint64 startup_time;
};

// Declare the global instance as extern
extern struct syscall_stats syscall_stats;

void update_syscall_stats(void);
void init_syscall_stats(void);

#endif