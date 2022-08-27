from base64 import encode
import socket
import rsa
import sys

public_key, private_key = rsa.newkeys(1024) # generate the keys for the sharing the data 
#getting the connection...
remote_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
remote_server.connect(("0.0.0.0", 54154))
remote_server.send(private_key.save_pkcs1('PEM')) #sending the key to server..
fhand = open(sys.argv[1], "r")
for line in fhand:
    encrypted_data = rsa.encrypt(line.encode("utf-8"), public_key)
    remote_server.send(encrypted_data)
    print(remote_server.recv(5000).decode('utf-8'))
remote_server.close()