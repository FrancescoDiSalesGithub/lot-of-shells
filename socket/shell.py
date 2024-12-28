import socket
import subprocess

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 5101       # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        conn, addr = s.accept()
        with conn:
                print('Connected by', addr)
                while True:
                   data = conn.recv(1024).decode('utf-8')
                   if not data:
                       break
                   print('Received:', data)
                
                   sysdata =  subprocess.run(data.replace("\n",""), capture_output=True, text=True)
                   conn.sendall(">: ".encode('utf-8')+sysdata.stdout.encode('utf-8'))  # Echo back the received data
