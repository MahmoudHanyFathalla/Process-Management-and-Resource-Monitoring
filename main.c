#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <bits/signum-generic.h>

// Function to calculate factorial
long long factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}
// Function to get CPU usage 
float get_cpu_usage() {
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        return -1;
    }

    // Variables to store CPU times
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    // Read CPU times from the first line in /proc/stat
    if (fscanf(fp, "cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
               &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice) != 10) {
        fclose(fp);
        perror("Failed to read CPU usage from /proc/stat");
        return -1;
    }

    fclose(fp);

    // Calculate total CPU time
    long total_cpu_time = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;

    // Calculate idle time (sum of idle and iowait)
    long total_idle_time = idle + iowait;

    // Calculate CPU usage percentage
    float cpu_usage = 100.0 * (1.0 - ((float)total_idle_time / total_cpu_time));
    
    return cpu_usage;
}

// Function to get CPU usage using top command
float get_cpu_usage_top() {
    FILE *fp = popen("top -bn1 | grep 'Cpu(s)' | sed 's/.*, *\\([0-9.]*\\)%* id.*/\\1/' | awk '{print 100 - $1}'", "r");
    if (fp == NULL) {
        perror("Failed to execute top command");
        return -1;
    }

    float cpu_usage;
    if (fscanf(fp, "%f", &cpu_usage) != 1) {
        perror("Failed to read CPU usage from command output");
        pclose(fp);
        return -1;
    }

    pclose(fp);

    return cpu_usage;
}

// Function to get RAM usage using free command
int get_ram_usage() {
    FILE *fp = popen("free -m | awk 'NR==2{print $3}'", "r");
    if (fp == NULL) {
        perror("Failed to execute free command");
        return -1;
    }

    int ram_usage;
    if (fscanf(fp, "%d", &ram_usage) != 1) {
        perror("Failed to read RAM usage from command output");
        pclose(fp);
        return -1;
    }

    pclose(fp);

    return ram_usage;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of child processes>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_children = atoi(argv[1]);
    if (num_children <= 0) {
        fprintf(stderr, "Number of child processes must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    pid_t *child_pids = malloc(num_children * sizeof(pid_t));
    if (child_pids == NULL) {
        perror("Failed to allocate memory for child process IDs");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to create child process");
            for (int j = 0; j < i; j++) {
                kill(child_pids[j], SIGKILL);
            }
            free(child_pids);
            return EXIT_FAILURE;
        } else if (pid == 0) {
            if (i == 0) {
                int number = 5; // Example number
                long long fact = factorial(number);
                printf("Child %d: Factorial of %d is %lld\n", getpid(), number, fact);
            } else if (i == 1) {
                int ram_usage = get_ram_usage();
                if (ram_usage != -1) {
                    printf("Child %d: RAM usage: %d MB\n", getpid(), ram_usage);
                } else {
                    printf("Failed to get RAM usage\n");
                }
            } else if (i == 2) {
                float cpu_usage = get_cpu_usage();
                if (cpu_usage != -1) {
                    printf("Child %d: CPU usage: %.2f%%\n", getpid(), cpu_usage);
                } else {
                    printf("Failed to get CPU usage\n");
                }
            }
            exit(EXIT_SUCCESS);
        } else {
            child_pids[i] = pid;
        }
    }

    for (int i = 0; i < num_children; i++) {
        waitpid(child_pids[i], NULL, 0);
    }

    free(child_pids);
    return EXIT_SUCCESS;
}
