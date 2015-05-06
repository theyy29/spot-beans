#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

void c_unknown(Command *c);
void c_echo(Command *c);
void c_info(Command *c);

/*
operation:play
  data:song
    origin:search
      track-id:####
    origin:playlist
      playlist-id:####
        track-id####
    origin:artist
      artist-id:####
        track-id:####
    origin:album
      album-id:####
        track-id:####
  data:playlist
    playlist-id:####
      [start-track-id:####]
  data:artist
    artist-id:####
      [start-track-id:####]
  data:album
    album-id:####
      [start-track-id:####]
operation:get-data
  data:playlist
    playlist-id:####
  data:playlists
  data:now-playing
operation:toggle-play-pause
operation:manage
  function:toggle-play-pause
  function:next-song
  function:previous-song
  function:restart-song
operation:search
  data:string
  ...
operation:edit
  data:playlist
    ...
  data:queue
    ...
*/

#endif // ifndef COMMANDS_H_INCLUDED
