#ifndef COMMON_H
#define COMMON_H

// Maximum number of clients that can connect to the server
#define MAX_CLIENTS 10

// Maximum message size
#define MESSAGE_SIZE 256

// Maximum length of a username
#define MAX_USERNAME_LENGTH 20

// Name of the server's named pipe (FIFO)
#define SERVER_PIPE_NAME "chat_fifo"

// Message types
#define TEXT_MESSAGE 1
#define JOIN_MESSAGE 2
#define LEAVE_MESSAGE 3

// Command codes
#define COMMAND_HELP "/help"
#define COMMAND_EXIT "/exit"
#define COMMAND_WHO "/who"

// Error codes
#define ERROR_INVALID_INPUT 1
#define ERROR_SERVER_FULL 2
#define ERROR_AUTH_FAILURE 3

// Authentication status
#define AUTH_SUCCESS 0
#define AUTH_FAILURE 1

#endif
