import socket

local_ip = "192.168.1.168"  # Raspberry Pi IP
local_port = 8080           # Port to listen on

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((local_ip, local_port))

print("Listening for responses...")
while True:
    data, addr = sock.recvfrom(1024)  # Buffer size 1024 bytes
    print(f"Received response: {data.decode()} from {addr}")
