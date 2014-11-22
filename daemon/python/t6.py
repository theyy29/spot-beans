import spotify
import threading
import time
import socket



logged_in_event = threading.Event()
def connection_state_listener(session):
    if session.connection.state is spotify.ConnectionState.LOGGED_IN:
        logged_in_event.set()

config = spotify.Config()
config.user_agent = "Spot-Beans"

session = spotify.Session(config);
session.on(spotify.SessionEvent.CONNECTION_STATE_UPDATED, connection_state_listener)


# Log in
session.login("disambiguity", "spot-beans");

# Init audio
audio = spotify.PortAudioSink(session)
loop = spotify.EventLoop(session)
loop.start()

while not logged_in_event.wait(0.1):
    session.process_events()

#search = session.search("Still loving you");
#search = session.search("time pink floyd");
#search = session.search("Burnin for you blue oyster cult");
search = session.search("I can't drive 55");

search.load()

print(search.tracks[0].load().name);
song = search.tracks[0]
song.load()
session.player.load(song)
session.player.play()

session.player.pause()
session.player.play()


###################################################

#create an INET, STREAMing socket
serversocket = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host,
# and a well-known port
serversocket.bind(("localhost", 33889))
#serversocket.bind((socket.gethostname(), 33889))
#become a server socket
serversocket.listen(5)

#(clientsocket, address) = serversocket.accept()
#s = clientsocket.recv(2048);
#print("Received: \"" + s + "\"")
#clientsocket.send("1");
#
#clientsocket.close();

#while 1:
    #accept connections from outside
    #(clientsocket, address) = serversocket.accept()
    #now do something with the clientsocket
    #in this case, we'll pretend this is a threaded server
    #ct = client_thread(clientsocket)
    #ct.run()

playing = True
playing = False
session.player.pause()

while 1:
    (clientsocket, address) = serversocket.accept()
    s = clientsocket.recv(2048);
    #######################################################################
    # Parse input
    #######################################################################
    print("Received: \"" + s + "\"")
    s = s.rstrip()
    com = s.split(' ')
    (a, b) = com[0].split(':', 1)
    print("Operation: \"" + b + "\"")
    if b == "get-data" :
        (c, d) = com[1].split(':', 1)
        print("data: \"" + d + "\"")
        if d == "playlists":
            clientsocket.send('{"playlists":[{"playlistname":\"Playlist1\", "playlistid":\"12381923\"},{"playlistname":\"Playlist2\", "playlistid":\"109823918\"}]}')
            #spotify:user:disambiguity:playlist:7vAroQ7fcmNSIhkouplZrf
        elif d == "now-playing":
            clientsocket.send('{"songs":[{"track":"Time","track-id":"129317189","artist":"Pink Floyd","album":"Dark Side of the Moon","albumartwork":""}]}')
        elif d == "playlist":
            clientsocket.send('{"songs":[{"track":"Time","trackid":"985329864","artist":"Pink Floyd","album":"Dark Side of the Moon","albumartwork":"","duration":100},\
                                         {"track":"Time","trackid":"123198233","artist":"Pink Floyd","album":"Dark Side of the Moon","albumartwork":"","duration":100},\
                                         {"track":"Time","trackid":"129317189","artist":"Pink Floyd","album":"Dark Side of the Moon","albumartwork":"","duration":100}]}')
        clientsocket.close()
    elif b == "toggle-play-pause" :
        clientsocket.send("1")
        clientsocket.close()
        if playing:
            playing = False
            session.player.pause()
        else:
            playing = True
            session.player.play()

    clientsocket.close();

    #time.sleep(100);

