# Makefile

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -pthread
LDFLAGS = -pthread

# Source files for the server and client
SERVER_SOURCES = server.c common.c
CLIENT_SOURCES = client.c common.c

# Executable names for the server and client
SERVER_EXECUTABLE = server
CLIENT_EXECUTABLE = client

# Phony targets (targets that are not associated with files)
.PHONY: all clean

# Default target: Build both server and client executables
all: $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)

# Rule to build the server executable
$(SERVER_EXECUTABLE): $(SERVER_SOURCES)
	$(CC) $(CFLAGS) $(SERVER_SOURCES) -o $(SERVER_EXECUTABLE) $(LDFLAGS)

# Rule to build the client executable
$(CLIENT_EXECUTABLE): $(CLIENT_SOURCES)
	$(CC) $(CFLAGS) $(CLIENT_SOURCES) -o $(CLIENT_EXECUTABLE) $(LDFLAGS)

# Clean target: Remove the executables
clean:
	rm -f $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)
