#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h" // Now included directly here
#include "proc.h"
#include "syscall_stats.h"
#include "defs.h"

// Remove the local declaration of syscall_stats (if present)
// The extern declaration comes from the header now

void update_syscall_stats(void)
{
    acquire(&syscall_stats.lock);

    if (syscall_stats.startup_time == 0)
    {
        syscall_stats.startup_time = r_time();
    }

    uint64 current_time = r_time();
    uint64 elapsed_seconds = (current_time - syscall_stats.startup_time) / 1000000;
    int current_minute = elapsed_seconds / 60;
    int current_hour = elapsed_seconds / 3600;

    if (current_minute != syscall_stats.last_minute)
    {
        syscall_stats.calls_per_minute[current_minute % 60] = syscall_stats.call_count;
        syscall_stats.call_count = 0;
        syscall_stats.last_minute = current_minute;
    }

    if (current_hour != syscall_stats.current_hour)
    {
        syscall_stats.hours_tracked++;
        syscall_stats.current_hour = current_hour;
        syscall_stats.calls_this_hour = 0;
    }

    syscall_stats.call_count++;
    syscall_stats.calls_this_hour++;

    release(&syscall_stats.lock);
}