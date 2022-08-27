from http import client
import socket
import rsa
import io
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("0.0.0.0", 54154))
server.listen(10)

while True:
    client_data = ""
    print("Waiting for the connection...")
    conn, addr = server.accept()
    print("Got Connection...")
    #get the key for the decryption
    data = conn.recv(1024)
    private_key = rsa.PrivateKey.load_pkcs1(data)
    
    while True:
        data = conn.recv(5000)
        if data == b'':
            break
        conn.send(rsa.decrypt(data, private_key))
    conn.close()

    