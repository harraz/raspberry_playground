import socket
import sys

esp8266_ip = '192.168.1.155'
port = 12345

message = sys.argv[1]

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(message.encode(), (esp8266_ip, port))
print("Message sent")
sock.close()

