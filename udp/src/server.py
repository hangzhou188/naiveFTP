import socket

size = 8192
seq = 0
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 9876))

try:
  while True:
    data, address = sock.recvfrom(size)
    sock.sendto(str(seq) + ' ' + data , address)
    seq = seq + 1
finally:
  sock.close()