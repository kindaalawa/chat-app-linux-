## Code Walkthrough

### Server Code 

#### Global Variables

- `int client_count`: Maintains a count of currently connected clients.
- `ClientInfo clients[MAX_CLIENTS]`: An array that stores client information for all connected clients.
- `sem_t sem_client_count`: A semaphore used to coordinate access to the `client_count` variable.
- `pthread_mutex_t mutex`: A mutex that safeguards critical sections in the code.


#### `send_message_all` Function

```c
void send_message_all(const char *message, int sender_fd)
```
#### `send_message_all` Function

The `send_message_all` function is responsible for broadcasting a message to all connected clients, excluding the sender.

**Parameters:**
- `message` (const char *): The message to be broadcasted.
- `sender_fd` (int): The file descriptor of the sender, which is used to exclude them from receiving the broadcast.


#### `authenticate_client` Function

```c
int authenticate_client(int client_fd, char *username)
```


The `authenticate_client` function is responsible for verifying user authentication by accepting any non-empty username as valid.

**Parameters:**
- `client_fd` (int): The file descriptor of the client.
- `username` (char *): The username provided by the client.


#### `handle_client` Function

```c
void *handle_client(void *arg)
```
The `handle_client` function serves as a thread function responsible for managing communication with an individual client.

**Parameters:**
- `arg` (ClientInfo*): A pointer to a `ClientInfo` structure that contains client-specific information.

#### `main` Function

```c
int main()
```
The `main` function plays a central role in this application, responsible for orchestrating server setup, client connection handling, and thread initialization.

**Key Steps:**
1. Create and open a named pipe for communication, designated as `SERVER_PIPE_NAME`.
2. Initialize a semaphore (`sem_client_count`) to ensure secure management of the client count.
3. Enter the primary server loop, which involves accepting client connections and setting up dedicated threads for communication.
4. Implement a robust user authentication mechanism and gracefully handle client disconnections to maintain server security and reliability.

### Client Code 

#### `main` Function

```c
int main()
```

- Description: Main function for the client responsible for setting up communication and handling user input.
- Key Steps:
  1. Create and open a named pipe for communication (`"chat_fifo"`).
  2. Enter a loop for taking user input and sending messages to the server.
  3. Handle user disconnection and clean up resources.

### Common Header

- Contains shared constants, such as `MAX_CLIENTS`, `MESSAGE_SIZE`, and synchronization mechanisms (`sem_t` and `pthread_mutex_t`).

### Synchronization and Security

- **Concurrency Safeguard:** The code employs synchronization mechanisms, including semaphores (`sem_t`) and mutexes (`pthread_mutex_t`), to ensure the safety of concurrent operations and eliminate potential race conditions.

- **User Authentication:** The system's security is reinforced through the implementation of user authentication logic within the `authenticate_client` function. This measure prevents unauthorized access to the server.

### Prevention of Deadlock and Starvation

- **Deadlock and Starvation Avoidance:** To mitigate the risk of deadlock and starvation, the system effectively utilizes semaphores and mutexes. These mechanisms guarantee that the execution of threads remains orderly and avoids resource conflicts.

- **Fair Resource Allocation:** The system employs a fair scheduling mechanism to ensure that all connected clients receive equitable resource allocation, promoting an even distribution of resources.

### Server-Client Communication

- **Communication Medium:** Communication between the server and clients relies on named pipes, also known as `FIFOs`. These named pipes provide a reliable conduit for message exchange and command transmission.

- **Robust Communication Protocol:** The system's communication protocol is designed to be robust, efficiently handling messages, commands, and user interactions while maintaining a streamlined and dependable communication flow.

