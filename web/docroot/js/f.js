function togglePlayPause(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "toggle-playing"
        },
        type: "POST",
    }).done(function(result){
        if(result == "1") {
            $("#song-control-toggle").removeClass("glyphicon-pause");
            $("#song-control-toggle").addClass("glyphicon-play");
        }
    });
}

function getPlaylists(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "get-data",
            data: "playlists"
        },
        type: "POST",
    }).done(function(result){
        if(result) {
            var playlists = JSON.parse(result);
            for (var i = 0; i < playlists["playlists"].length; i++) {

                var playlistName = playlists["playlists"][i].playlistname;
                var playlistId   = playlists["playlists"][i].playlistid;

                var li = "<li id=\"" + playlistId + "\"><a href=\"#" + playlistId + "\">" + playlistName + "</a></li>";
                $("#playlist-list ul").append(li);
            }
        }
    });
}

function getSongList(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "get-data",
            data: "playlist"
            // NOTE: NEED TO ADD PLAYLIST ID
        },
        type: "POST",
    }).done(function(result){
        if(result) {
            var songs = JSON.parse(result);
            for (var i = 0; i < songs["songs"].length; i++) {

                var track = songs["songs"][i].track;
                var trackId = songs["songs"][i].trackid;
                var artist = songs["songs"][i].artist;
                var album = songs["songs"][i].album;
                var albumArtWork = songs["songs"][i].albumartwork;
                var duration = songs["songs"][i].duration;

                var li = "\
                    <li id=\"" + trackId + "\">\
                        <span class=\"glyphicon glyphicon-play btn btn-primary btn-sm song-play\"></span>\
                        <span class=\"song-name\">" + track + "</span>\
                        <span class=\"song-artist\">" + artist + "</span>\
                        <span class=\"song-album\">" + album + "</span>\
                        <span class=\"song-time\">" + duration + "</span>\
                    </li>\
                ";
                $("#playlist-song-list ul").append(li);
            }
        }
    });
}