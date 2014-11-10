

#create an INET, STREAMing socket
#serversocket = socket.socket(
    #socket.AF_INET, socket.SOCK_STREAM)
##bind the socket to a public host,
#serversocket.bind(("localhost", 33889))
##become a server socket
#serversocket.listen(5)
#
############################################################################
#
##Set up polling
#poller = select.poll()
#poller.register(serversocket, select.POLLPRI | select.POLLIN | select.PULLHUP)
#
#
#fd_to_socket = { serversocket.fileno(): serversocket }
#
###########################################################################


#else:
    #print("Time elapased:" + (playinginfo[3] - time.time()))
    # Update the song

#time.sleep(100);


###########################################################################
###########################################################################
###########################################################################
###########################################################################
import select
import socket
import sys
import Queue
import spotify
import threading
import time
import codecs
import time # we need more to work on the project...


###########################################################################



###### SPOTIFY START ######

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
#search = session.search("I can't drive 55");
#
#search.load()


playlist = session.get_playlist("spotify:user:disambiguity:playlist:7vAroQ7fcmNSIhkouplZrf")
pls = session.playlist_container
pls.load()
#playlist = session.get_playlist("spotify:user:mister923:playlist:7D60P3T5BP3VMcViZc65nd")
#spotify:user:disambiguity:playlist:7vAroQ7fcmNSIhkouplZrf
#https://play.spotify.com/user/mister923/playlist/7D60P3T5BP3VMcViZc65nd
playlist.load()
print(playlist.is_in_ram)
playlist.set_in_ram(True)
playlist.load()
#playlist.tracks
#playlist.name
#playlist.link

print(playlist.is_loaded)
print(len(playlist.tracks))

song = playlist.tracks[0]
song.load()
session.player.load(song)
session.player.play()
starttime = time.time()
session.player.prefetch(playlist.tracks[1])

###########################################################################

# We are [1] playing, [0] at the 0th track, from the [playlist] playlist,
#       which started playing at [starttime]
playinginfo = [1, 0, playlist, starttime]

###########################################################################

playing = True
#playing = False
#session.player.pause()


###### SPOTIFY END ########################################################



# Create a TCP/IP socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setblocking(0)

# Bind the socket to the port
server_address = ('localhost', 33889)
print >>sys.stderr, 'starting up on %s port %s' % server_address
server.bind(server_address)

# Listen for incoming connections
server.listen(5)

# Keep up with the queues of outgoing messages
message_queues = {}

###

# Do not block forever (milliseconds)
TIMEOUT = 1000

###

# Commonly used flag setes
READ_ONLY = select.POLLIN | select.POLLPRI | select.POLLHUP | select.POLLERR
READ_WRITE = READ_ONLY | select.POLLOUT

###


# Set up the poller
poller = select.poll()
poller.register(server, READ_ONLY)

###

# Map file descriptors to socket objects
fd_to_socket = { server.fileno(): server,
}


def readnreply(clientsocket):
    global playlist
    global pls
    global song
    global playinginfo
    global session
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
            clientsocket.send('{"playlists":[');
            clientsocket.send('{"playlistname":"' + pls[0].name + '", "playlistid":"' + pls[0].link.uri + '"}')
            for pl in pls[1:]:
                clientsocket.send(',{"playlistname":"' + pl.name + '", "playlistid":"' + pl.link.uri + '"}')
            clientsocket.send(']}');
        elif d == "now-playing":
            clientsocket.send('{"songs":[{"track":"'            + song.name + 
                                '","trackid":"'                   + song.link.uri + 
                                '","artist":"'                    + song.artists[0].load().name + 
                                '","album":"'                     + song.album.load().name + 
                                '","albumartwork":"'              + song.album.cover().load().data_uri + 
                                '","duration":"'                  + str(song.duration) + '"}],' +
                                '"playlists":[{"playlistname":"'  + playlist.name +
                                '","playlist":"'                  + playlist.link.uri + '"}]}')
        elif d == "playlist":
            playlist = ""
            for p in pls:
                (g, h) = com[2].split(':', 1) # g == id hopefully
                #h = com[2][9:]
                print("ID:" + h)
                if p.link.uri == h:
                    playlist = p
                    break
            clientsocket.send('{"songs":[')
#clientsocket.send('{"track":"' +playlist.tracks[0].name + '","trackid":"' + playlist.tracks[0].link.uri + '","artist":"' + playlist.tracks[0].artists[0].load().name + '","album":"' + playlist.tracks[0].album.load().name + '","duration":"' + str(playlist.tracks[0].duration) + '"}')
            clientsocket.send('{"track":"'          + playlist.tracks[0].name + 
                        '","trackid":"'             + playlist.tracks[0].link.uri + 
                        '","artist":"'              + playlist.tracks[0].artists[0].load().name + 
                        '","album":"'               + playlist.tracks[0].album.load().name + 
                        '","albumartwork":"'        + playlist.tracks[0].album.cover().load().data_uri + 
                        '","duration":"'            + str(playlist.tracks[0].duration) + '"}')
            for so in playlist.tracks[1:]:
                clientsocket.send(',{"track":"'         + so.name +
                                '","trackid":"'       + so.link.uri + 
                                '","artist":"'        + so.artists[0].load().name + 
                                '","album":"'         + so.album.load().name + 
                                '","albumartwork":"'  + so.album.cover().load().data_uri + 
                                '","duration":"'      + str(so.duration) + '"}')
            clientsocket.send(']}')
        elif d == "is-playing":
            if playing:
                clientsocket.send("1")
            else:
                clientsocket.send("0")
        elif d == "current-time":
            clientsocket.send("0");
        clientsocket.close()
    elif b == "toggle-play-pause" :
        clientsocket.send("1")
        clientsocket.close()
        if playinginfo[0]:
            playinginfo[0] = False
            session.player.pause()
        else:
            playinginfo[0] = True
            #session.player.play()
    elif b == "play":
        (c, d) = com[1].split(':', 1)
        print("data: \"" + d + "\"")


    clientsocket.close();

###

while True:

    # Wait for at least one of the sockets to be ready for processing
    print >>sys.stderr, '\nwaiting for the next event'
    events = poller.poll(TIMEOUT)

    for fd, flag in events:

    # Retrieve the actual socket from its file descriptor
        s = fd_to_socket[fd]
        # Handle inputs
        if flag & (select.POLLIN | select.POLLPRI):

            if s is server:
                # A "readable" server socket is ready to accept a connection
                connection, client_address = s.accept()
                print >>sys.stderr, 'new connection from', client_address
                connection.setblocking(0)
                fd_to_socket[ connection.fileno() ] = connection
                poller.register(connection, READ_ONLY)

                # Give the connection a queue for data we want to send
                message_queues[connection] = Queue.Queue()

            else:
                readnreply(s) #################################### HANDLE THE REQUEST
                continue
                data = s.recv(1024)
                if data:
                    # A readable client socket has data
                    print >>sys.stderr, 'received "%s" from %s' % (data, s.getpeername())
                    message_queues[s].put(data)
                    # Add output channel for response
                    poller.modify(s, READ_WRITE)

                else:
                    # Interpret empty result as closed connection
                    print >>sys.stderr, 'closing', client_address, 'after reading no data'
                    # Stop listening for input on the connection
                    poller.unregister(s)
                    s.close()

                    # Remove message queue
                del message_queues[s]


        elif flag & select.POLLHUP:
            # Client hung up
            print >>sys.stderr, 'closing', client_address, 'after receiving HUP'
            # Stop listening for input on the connection
            poller.unregister(s)
            s.close()


        elif flag & select.POLLOUT:
            # Socket is ready to send data, if there is any to send.
            try:
                next_msg = message_queues[s].get_nowait()
            except Queue.Empty:
                # No messages waiting so stop checking for writability.
                print >>sys.stderr, 'output queue for', s.getpeername(), 'is empty'
                poller.modify(s, READ_ONLY)
            else:
                print >>sys.stderr, 'sending "%s" to %s' % (next_msg, s.getpeername())
                s.send(next_msg)


        elif flag & select.POLLERR:
            print >>sys.stderr, 'handling exceptional condition for', s.getpeername()
            # Stop listening for input on the connection
            poller.unregister(s)
            s.close()

            # Remove message queue
        #del message_queues[s]



