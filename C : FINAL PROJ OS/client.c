#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    // Create named pipe (FIFO) for communication
    mkfifo("chat_fifo", 0666);

    // Open the pipe for reading and writing
    int client_fd = open("chat_fifo", O_RDWR);

    // Implement user authentication
    char username[MAX_USERNAME_LENGTH];
    printf("Enter your username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    write(client_fd, username, strlen(username));

    // Receive authentication result
    int auth_result;
    read(client_fd, &auth_result, sizeof(auth_result));
    if (auth_result == ERROR_AUTH_FAILURE) {
        printf("Authentication failed. Exiting...\n");
        close(client_fd);
        unlink("chat_fifo"); // Remove the named pipe
        return 1;
    }

    // Main message input loop
    char message[MESSAGE_SIZE];
    while (1) {
        // Input message from the user
        printf("Enter message: ");
        fgets(message, MESSAGE_SIZE, stdin);

        // Send the message to the server
        write(client_fd, message, strlen(message));

        // Handle disconnection and exit command
        if (strncmp(message, COMMAND_EXIT, strlen(COMMAND_EXIT)) == 0) {
            printf("Disconnecting from the server.\n");
            break;
        }
    }

    // Clean up resources
    close(client_fd);
    unlink("chat_fifo"); // Remove the named pipe

    return 0;
}
