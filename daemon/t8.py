import spotify
import threading
import time
import socket
import codecs


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
session.player.preload(playlist.tracks[1])

###########################################################################

# We are [1] playing, [0] at the 0th track, from the [playlist] playlist,
#       which started playing at [starttime]
playinginfo = [1, 0, playlist, starttime]

###########################################################################

#create an INET, STREAMing socket
serversocket = socket.socket(
    socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host,
serversocket.bind(("localhost", 33889))
#become a server socket
serversocket.listen(5)


playing = True
#playing = False
#session.player.pause()


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
            clientsocket.send('{"songs":[')
            #clientsocket.send('{"track":"' +playlist.tracks[0].name + '","trackid":"' + playlist.tracks[0].link.uri + '","artist":"' + playlist.tracks[0].artists[0].load().name + '","album":"' + playlist.tracks[0].album.load().name + '","duration":"' + str(playlist.tracks[0].duration) + '"}')
            clientsocket.send('{"track":"'          +playlist.tracks[0].name + 
                              '","trackid":"'       + playlist.tracks[0].link.uri + 
                              '","artist":"'        + playlist.tracks[0].artists[0].load().name + 
                              '","album":"'         + playlist.tracks[0].album.load().name + 
                              '","albumartwork":"'  + playlist.tracks[0].album.cover().load().data_uri + 
                              '","duration":"'      + str(playlist.tracks[0].duration) + '"}')
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
            clientsocket.write("0");
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

