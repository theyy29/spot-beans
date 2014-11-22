import socket


#create an INET, STREAMing socket
serversocket = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host,
# and a well-known port
serversocket.bind(("localhost", 33889))
#serversocket.bind((socket.gethostname(), 33889))
#become a server socket
serversocket.listen(5)

(clientsocket, address) = serversocket.accept()
s = clientsocket.recv(2048);
print("Received: \"" + s + "\"")
clientsocket.send("1");

clientsocket.close();

#while 1:
    #accept connections from outside
    #(clientsocket, address) = serversocket.accept()
    #now do something with the clientsocket
    #in this case, we'll pretend this is a threaded server
    #ct = client_thread(clientsocket)
    #ct.run()
