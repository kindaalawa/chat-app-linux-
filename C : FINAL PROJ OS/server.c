#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Structure to hold client information
typedef struct {
    int client_fd;
    char username[MAX_USERNAME_LENGTH];
} ClientInfo;

int client_count = 0;
ClientInfo clients[MAX_CLIENTS];
sem_t sem_client_count;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to send a message to all connected clients except the sender
void send_message_all(const char *message, int sender_fd) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].client_fd != -1 && clients[i].client_fd != sender_fd) {
            write(clients[i].client_fd, message, strlen(message));
        }
    }
    pthread_mutex_unlock(&mutex);
}

// Function to perform user authentication
int authenticate_client(int client_fd, char *username) {
    // User authentication logic: Any non-empty username is considered valid
    if (strlen(username) > 0) {
        return AUTH_SUCCESS;
    } else {
        return AUTH_FAILURE;
    }
}

// Thread function for handling communication with an individual client
void *handle_client(void *arg) {
    ClientInfo *client_info = (ClientInfo *)arg;
    int client_fd = client_info->client_fd;
    char username[MAX_USERNAME_LENGTH];
    strcpy(username, client_info->username);

    // Notify other clients about the new user
    char join_message[MESSAGE_SIZE];
    snprintf(join_message, sizeof(join_message), "[%s has joined the chat.]\n", username);
    send_message_all(join_message, client_fd);

    // Main message handling loop
    char message[MESSAGE_SIZE];
    while (1) {
        int bytes_read = read(client_fd, message, MESSAGE_SIZE - 1);
        if (bytes_read <= 0) {
            // Handle client disconnection
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].client_fd == client_fd) {
                    clients[i].client_fd = -1;
                    sem_wait(&sem_client_count);
                    client_count--;
                    sem_post(&sem_client_count);
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);

            // Notify other clients about the user leaving
            char leave_message[MESSAGE_SIZE];
            snprintf(leave_message, sizeof(leave_message), "[%s has left the chat.]\n", username);
            send_message_all(leave_message, client_fd);

            close(client_fd);
            free(client_info);
            pthread_exit(NULL);
        }

        message[bytes_read] = '\0';

        // Handle commands
        if (strncmp(message, COMMAND_HELP, strlen(COMMAND_HELP)) == 0) {
            char help_message[] = "Commands:\n/help - Show this help message\n/who - List connected users\n/exit - Leave the chat\n";
            write(client_fd, help_message, strlen(help_message));
        } else if (strncmp(message, COMMAND_WHO, strlen(COMMAND_WHO)) == 0) {
            char who_message[MESSAGE_SIZE];
            snprintf(who_message, sizeof(who_message), "Connected users: %d\n", client_count);
            write(client_fd, who_message, strlen(who_message));
        } else if (strncmp(message, COMMAND_EXIT, strlen(COMMAND_EXIT)) == 0) {
            // Exit command breaks the loop
            break;
        } else {
            // Broadcast regular text message
            char broadcast_message[MESSAGE_SIZE];
            snprintf(broadcast_message, sizeof(broadcast_message), "[%s]: %s", username, message);
            send_message_all(broadcast_message, client_fd);
        }
    }

    // Clean up resources
    close(client_fd);
    free(client_info);
    pthread_exit(NULL);
}

int main() {
    // Create a named pipe (FIFO)
    mkfifo(SERVER_PIPE_NAME, 0666);

    // Open the pipe for reading and writing
    int server_fd = open(SERVER_PIPE_NAME, O_RDWR);

    // Initialize a semaphore to protect the client_count variable
    sem_init(&sem_client_count, 0, 1);

    // Main server loop
    //creates a server that continuously accepts incoming 
    //client connections in an infinite loop. If an error occurs 
    //during the acceptance process, an error message is printed, 
    //and the server continues listening for new connections.
    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0) {
            perror("Error accepting client");
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (client_count < MAX_CLIENTS) {
            
                if (clients[i].client_fd == -1) {
                    // Initialize client info
                    clients[i].client_fd = client_fd;

                    // Prompt for username
                    write(client_fd, "Enter your username: ", 21);
                    char username[MAX_USERNAME_LENGTH];
                    int auth_result = ERROR_AUTH_FAILURE;
                    while (auth_result == ERROR_AUTH_FAILURE) {
                        int bytes_read = read(client_fd, username, MAX_USERNAME_LENGTH - 1);
                        if (bytes_read > 0) {
                            username[bytes_read - 1] = '\0'; // Remove newline character
                            auth_result = authenticate_client(client_fd, username);
                            if (auth_result == AUTH_SUCCESS) {
                                strncpy(clients[i].username, username, MAX_USERNAME_LENGTH);
                                sem_wait(&sem_client_count);
                                client_count++;
                                sem_post(&sem_client_count);
                                pthread_t client_thread;
                                ClientInfo *client_info = malloc(sizeof(ClientInfo));
                                client_info->client_fd = client_fd;
                                strncpy(client_info->username, username, MAX_USERNAME_LENGTH);
                                pthread_create(&client_thread, NULL, handle_client, (void *)client_info);
                                break;
                            } else {
                                write(client_fd, "Invalid username. Please try again: ", 37);
                            }
                        } else {
                            // Handle read error
                            break;
                        }
                    }
                    break;
                }
            
        } else {
            // Reject the client if the server is at maximum capacity
            write(client_fd, "Server full. Try again later.\n", 30);
            close(client_fd);
        }
        pthread_mutex_unlock(&mutex);
    }

    // Clean up resources
    close(server_fd);
    unlink(SERVER_PIPE_NAME); // Remove the named pipe
    sem_destroy(&sem_client_count);

    return 0;
}
