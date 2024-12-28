
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX_BUFFER 1024

void handle_client(int client_socket) {
    char buffer[MAX_BUFFER];
    int bytes_read;

    while ((bytes_read = read(client_socket, buffer, MAX_BUFFER - 1)) > 0) {
        buffer[bytes_read] = '\0';

        printf("Received command: %s\n", buffer);

        // Execute the command using fork() and execvp()
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            char *args[MAX_BUFFER];
            int i = 0;
            args[i++] = strtok(buffer, " ");
            while ((args[i++] = strtok(NULL, " ")) != NULL);
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else if (pid > 0) {
            // Parent process
            wait(NULL); // Wait for child process to finish
        } else {
            perror("fork failed");
            exit(1);
        }
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket creation failed");
        exit(1);
    }

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("listen failed");
        close(server_socket);
        exit(1);
    }

    printf("Server started on port %d\n", PORT);

    while (1) {
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("accept failed");
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        // Handle client in a separate process
        if (fork() == 0) {
            close(server_socket);
            handle_client(client_socket);
            exit(0);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
