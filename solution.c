#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

int main() {
    // Get executable directory
    char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len == -1) {
        perror("readlink failed");
        return 1;
    }
    exe_path[len] = '\0';
    
    // Get directory containing executable
    char *last_slash = strrchr(exe_path, '/');
    if (last_slash) {
        *last_slash = '\0';
    }
    
    // Build interpreter path
    char interp_path[PATH_MAX];
    snprintf(interp_path, sizeof(interp_path), "%s/interpreter/interpreter", exe_path);
    
    // Create temporary file with .mv code
    char template[] = "/tmp/mvcode_XXXXXX";
    int fd = mkstemp(template);
    if (fd == -1) {
        perror("mkstemp failed");
        return 1;
    }
    
    // Write the .mv code
    const char *mv_code = 
        "# Hello World - Problem 2076\n"
        "# Output \"Hello World!\"\n"
        "O<72\n"
        "O<101\n"
        "O<108\n"
        "O<108\n"
        "O<111\n"
        "O<32\n"
        "O<87\n"
        "O<111\n"
        "O<114\n"
        "O<108\n"
        "O<100\n"
        "O<33\n"
        "Z<1\n";
    
    write(fd, mv_code, strlen(mv_code));
    close(fd);
    
    // Create temp file for input
    char input_template[] = "/tmp/mvinput_XXXXXX";
    int input_fd = mkstemp(input_template);
    if (input_fd == -1) {
        perror("mkstemp input failed");
        unlink(template);
        return 1;
    }
    
    // Copy stdin to temp input file
    char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        write(input_fd, buffer, bytes);
    }
    close(input_fd);
    
    // Execute interpreter
    char *args[] = {interp_path, template, input_template, NULL};
    execv(interp_path, args);
    
    // If exec fails
    perror("execv failed");
    unlink(template);
    unlink(input_template);
    return 1;
}
