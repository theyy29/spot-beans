

from spotify import spotify
import threading

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

while not logged_in_event.wait(0.1):
    session.process_events()

album = session.get_album('spotify:album:0XHp09qTpqJJQwa2zFxAAE')
album.load()
