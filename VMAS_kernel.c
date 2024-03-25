#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <dirent.h>
#include <string.h>

// Function to get virtual memory usage
void getVirtualMemoryUsage() {	
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("Virtual Memory Usage: %ld KB\n", usage.ru_maxrss);
    } else {
        printf("Failed to get virtual memory usage.\n");
    }
}

// Function to analyze page faults
void analyzePageFaults() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("Page Faults (soft page faults): %ld\n", usage.ru_minflt);
        printf("Page Faults (hard page faults): %ld\n", usage.ru_majflt);
    } else {
        printf("Failed to analyze page faults.\n");
    }
}

// Function to track memory allocations
void* customMalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr != NULL) {
        printf("Allocated %zu bytes at address %p\n", size, ptr);
    } else {
        printf("Memory allocation failed\n");
    }
    return ptr;
}

// Function to release memory allocations
void customFree(void* ptr) {
    free(ptr);
    printf("Freed memory at address %p\n", ptr);
}

// Function to plot memory usage over time using gnuplot
void plotMemoryUsage() {
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe != NULL) {
        fprintf(gnuplotPipe, "set title 'Memory Usage Over Time'\n");
        fprintf(gnuplotPipe, "set xlabel 'Time (s)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Memory Usage (KB)'\n");
        fprintf(gnuplotPipe, "plot 'memory_usage.dat' with linespoints\n");
        fclose(gnuplotPipe);
    } else {
        printf("Failed to open gnuplot pipe.\n");
    }
}

// Function to get memory usage for a process
void getProcessMemoryUsage(const char *pid) {
    char filename[256];
    snprintf(filename, sizeof(filename), "/proc/%s/statm", pid);
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        unsigned long size, resident, share, text, lib, data, dt;
        if (fscanf(fp, "%lu %lu %lu %lu %lu %lu %lu", &size, &resident, &share, &text, &lib, &data, &dt) == 7) {
            printf("Process ID: %s\n", pid);
            printf("Size: %lu pages\n", size);
            printf("Resident: %lu pages\n", resident);
            printf("Share: %lu pages\n", share);
            printf("Text: %lu pages\n", text);
            printf("Library: %lu pages\n", lib);
            printf("Data+Stack: %lu pages\n", data);
            printf("Dirty: %lu pages\n", dt);
            printf("\n");
        }
        fclose(fp);
    } else {
        printf("Failed to open file for process: %s\n", pid);
    }
}

// Function to get process information
void getProcessInfo() {
    printf("Getting information about processes...\n");
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("Failed to open /proc directory");
        exit(EXIT_FAILURE);
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0) {
            getProcessMemoryUsage(entry->d_name);
        }
    }
    closedir(dir);
}

int main() {
    // Example usage of the virtual memory analytics suite
    getVirtualMemoryUsage();
    analyzePageFaults();
    // Example memory allocation and deallocation
    int* arr = (int*)customMalloc(10 * sizeof(int));
    if (arr != NULL) {
        // Use allocated memory
        for (int i = 0; i < 10; ++i) {
            arr[i] = i;
        }
        // Free allocated memory
    customFree(arr);
    }SS
    // Plot memory usage over time
    plotMemoryUsage();
    // Get process information
    getProcessInfo();
    // Additional cases with user inputs
    printf("\nEnter the size for a new memory allocation (in bytes): ");
    size_t newSize;
    scanf("%zu", &newSize);
    // Example of user-defined memory allocation
    void* userAllocatedMemory = customMalloc(newSize);
    if (userAllocatedMemory != NULL) {
        // Use allocated memory
        printf("Do something interesting with the allocated memory!\n");
        // Free user-defined memory allocation
        customFree(userAllocatedMemory);
    }
    return 0;
}