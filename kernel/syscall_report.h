#ifndef _SYSCALL_REPORT_H
#define _SYSCALL_REPORT_H

struct syscall_report
{
    int calls_per_minute[60];
    int current_minute;
    int hourly_average;
    int hours_tracked;
};

#endif