# Process Management and Resource Monitoring

## Overview
This project demonstrates process creation, inter-process communication, and system resource monitoring in C. It creates multiple child processes, where each child performs a different task such as calculating factorial, monitoring CPU usage, and checking RAM usage.

## Features
- **Process management** using `fork()` and `waitpid()`
- **System resource monitoring** (CPU and RAM usage)
- **Dynamic memory allocation** for process tracking
- **Graceful error handling** for robust execution

## Prerequisites
Ensure you have a C compiler installed, such as `gcc`, and that your system supports process creation and `/proc` file access (Linux-based systems).

## Compilation and Execution
### Compile the Code
```sh
gcc -o main main.c
```

### Run the Program
```sh
./main <number_of_child_processes>
```

### Example Usage
```sh
./main 3
```

## Code Explanation
### 1. **Process Creation**
- The program creates a specified number of child processes.
- Each child process performs a unique task (factorial calculation, CPU usage monitoring, RAM usage monitoring).

### 2. **Factorial Calculation**
- The first child process calculates the factorial of a predefined number (`5`).

### 3. **System Resource Monitoring**
- The second child process retrieves and displays the system's RAM usage.
- The third child process calculates CPU usage from `/proc/stat` and an alternative command-based approach using `top`.

### 4. **Process Synchronization**
- Parent waits for all child processes to finish using `waitpid()`.
- If any child process fails to start, the program terminates gracefully, killing any previously created processes.

## Sample Output
```
Child 12345: Factorial of 5 is 120
Child 12346: RAM usage: 2048 MB
Child 12347: CPU usage: 15.32%
```

## Possible Enhancements
- Implement **real-time monitoring** with periodic updates.
- Add **inter-process communication (IPC)** for better data exchange.
- Enhance **error handling** for edge cases.

## Author
**Mahmoud Hany** - Computer Engineering Student & AI Enthusiast
