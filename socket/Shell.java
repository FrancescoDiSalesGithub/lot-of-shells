import java.io.*;
import java.net.*;

public class Shell {

    public static void main(String[] args) throws IOException {
        int portNumber = 8080; // Choose a port number

        try (ServerSocket serverSocket = new ServerSocket(portNumber)) {
            System.out.println("Server started on port " + portNumber);

            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Client connected: " + clientSocket.getInetAddress());

                // Handle client connection in a separate thread
                new Thread(() -> handleClient(clientSocket)).start();
            }
        } catch (IOException e) {
            System.err.println("Error starting server: " + e.getMessage());
        }
    }

    private static void handleClient(Socket clientSocket) {
        try (BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)) {

            String command;
            while ((command = in.readLine()) != null) {
                System.out.println("Received command: " + command);

                // Execute the command using Runtime.getRuntime().exec()
                Process process = Runtime.getRuntime().exec(command);

                // Read and send the output of the command to the client
                BufferedReader processOutput = new BufferedReader(new InputStreamReader(process.getInputStream()));
                String line;
                while ((line = processOutput.readLine()) != null) {
                    out.println(line);
                }

                // Handle any errors from the command execution
                BufferedReader processError = new BufferedReader(new InputStreamReader(process.getErrorStream()));
                while ((line = processError.readLine()) != null) {
                    out.println("Error: " + line);
                }

                process.waitFor();
            }

        } catch (IOException | InterruptedException e) {
            System.err.println("Error handling client: " + e.getMessage());
        } finally {
            try {
                clientSocket.close();
            } catch (IOException e) {
                System.err.println("Error closing client socket: " + e.getMessage());
            }
        }
    }
}
