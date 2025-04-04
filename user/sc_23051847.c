#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "syscall_report.h"

// Function declarations
void reverse(char *s);
void itoa(int n, char *s);
void strcat_custom(char *dest, const char *src);

// String reverse implementation
void reverse(char *s)
{
    int i, j;
    char c;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// Integer to string implementation
void itoa(int n, char *s)
{
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
    {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

// Custom string concatenation
void strcat_custom(char *dest, const char *src)
{
    while (*dest)
        dest++;
    while ((*dest++ = *src++))
        ;
}

int main(int argc, char *argv[])
{
    struct syscall_report report;

    if (sc_23051847(&report) < 0)
    {
        printf("System call failed\n");
        exit(1);
    }

    // Print statistics
    printf("System call hourly average: %d\n", report.hourly_average);
    printf("System calls per minute (last 60 minutes):\n");
    for (int i = 0; i < 60; i++)
    {
        int idx = (report.current_minute + 1 + i) % 60;
        printf("Minute %d: %d calls\n", i, report.calls_per_minute[idx]);
    }

    // Create data file
    int fd = open("syscall_data.txt", O_CREATE | O_WRONLY);
    if (fd < 0)
    {
        printf("Failed to create data file\n");
        exit(1);
    }

    char buffer[64];
    char num[12];
    for (int i = 0; i < 60; i++)
    {
        int idx = (report.current_minute + 1 + i) % 60;

        // Reset buffer
        buffer[0] = '\0';

        // Convert and append minute number
        itoa(i, num);
        strcat_custom(buffer, num);
        strcat_custom(buffer, " ");

        // Convert and append call count
        itoa(report.calls_per_minute[idx], num);
        strcat_custom(buffer, num);
        strcat_custom(buffer, "\n");

        write(fd, buffer, strlen(buffer));
    }

    close(fd);
    exit(0);
}