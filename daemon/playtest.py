

import spotify
import threading
import time



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
search = session.search("Burnin for you blue oyster cult");

search.load()

print(search.tracks[0].load().name);
song = search.tracks[0]
song.load()
session.player.load(song)
session.player.play()

session.player.pause()
session.player.play()

while 1:
    time.sleep(100);

