#ifndef COMMON_H
#define COMMON_H

// Maximum number of clients that can connect to the server
#define MAX_CLIENTS 10

// Maximum size of a message
#define MESSAGE_SIZE 256

// Maximum length of a username
#define MAX_USERNAME_LENGTH 20

// Name of the server's named pipe (FIFO)
#define SERVER_PIPE_NAME "chat_fifo"

// Message types
#define TEXT_MESSAGE 1   // Represents a regular text message
#define JOIN_MESSAGE 2   // Represents a join message when a client connects
#define LEAVE_MESSAGE 3  // Represents a leave message when a client disconnects

// Command codes
#define COMMAND_HELP "/help"   // Command to request help
#define COMMAND_EXIT "/exit"   // Command to exit the chat
#define COMMAND_WHO "/who"     // Command to list connected users

// Error codes
#define ERROR_INVALID_INPUT 1   // Error code for invalid input
#define ERROR_SERVER_FULL 2     // Error code when the server is full and cannot accept more clients
#define ERROR_AUTH_FAILURE 3    // Error code for authentication failure

// Authentication status
#define AUTH_SUCCESS 0    // Successful authentication status
#define AUTH_FAILURE 1    // Authentication failure status

#endif
