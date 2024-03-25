#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <dirent.h>
#include <string.h>

// Function to get virtual memory usage
void getVirtualMemoryUsage()
{
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0)
    {
        g_print("Virtual Memory Usage: %ld KB\n", usage.ru_maxrss);
    }
    else
    {
        g_print("Failed to get virtual memory usage.\n");
    }
}

// Function to analyze page faults
void analyzePageFaults()
{
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0)
    {
        g_print("Page Faults (soft page faults): %ld\n", usage.ru_minflt);
        g_print("Page Faults (hard page faults): %ld\n", usage.ru_majflt);
    }
    else
    {
        g_print("Failed to analyze page faults.\n");
    }
}

// Function to track memory allocations
void *customMalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr != NULL)
    {
        g_print("Allocated %zu bytes at address %p\n", size, ptr);
    }
    else
    {
        g_print("Memory allocation failed\n");
    }
    return ptr;
}

// Function to release memory allocations
void customFree(void *ptr)
{
    free(ptr);
    g_print("Freed memory at address %p\n", ptr);
}

// Callback function for "Allocate Memory" button
void allocateMemory(GtkWidget *widget, gpointer data)
{
    size_t size = (size_t)atoi(gtk_entry_get_text(GTK_ENTRY(data)));
    if (size <= 0)
    {
        g_print("Invalid memory size\n");
        return;
    }
    void *ptr = customMalloc(size);
    if (ptr != NULL)
    {
        // Use allocated memory
    }
}

int main(int argc, char *argv[])
{
    GtkWidget *window, *grid, *button, *entry;
    gtk_init(&argc, &argv);

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Memory Analyzer");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Add Virtual Memory Usage button
    button = gtk_button_new_with_label("Get Virtual Memory Usage");
    g_signal_connect(button, "clicked", G_CALLBACK(getVirtualMemoryUsage), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, S0, 0, 1, 1);

    // Add Analyze Page Faults button
    button = gtk_button_new_with_label("Analyze Page Faults");
    g_signal_connect(button, "clicked", G_CALLBACK(analyzePageFaults), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

    // Add entry for memory size
    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);

    // Add Allocate Memory button
    button = gtk_button_new_with_label("Allocate Memory");
    g_signal_connect(button, "clicked", G_CALLBACK(allocateMemory), entry);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);

    // Show all widgets
    gtk_widget_show_all(window);
    
    // Run the main loop
    gtk_main();
    return 0;
}