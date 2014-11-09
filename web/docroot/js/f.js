var percent = 0;
var fakeResult = 0;

function togglePlayPause(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "toggle-play-pause"
        },
        type: "POST",
    }).done(function(result){
        if(result == "1") { // if playing, show pause
            if ($("#song-control-toggle").hasClass("glyphicon-play")) { // if playing, show pause
                $("#song-control-toggle").addClass("glyphicon-pause");
                $("#song-control-toggle").removeClass("glyphicon-play");
            } else { // else, show play
                $("#song-control-toggle").removeClass("glyphicon-pause");
                $("#song-control-toggle").addClass("glyphicon-play");
            }
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

                var p = playlists["playlists"][i];

                var playlistName = p.playlistname;
                var playlistId   = p.playlistid;

                // console.log(playlistId);
                playlistIdClean = playlistId.replace(/:/g,''); // remove ":" from id

                var li = "<li id=\"" + playlistIdClean + "\"><a id=\"" + playlistId + "\" href=\"#\">" + playlistName + "</a></li>";
                $("#playlist-list ul").append(li);
            }
        }
        if(!ajaxDone["getPlaylists"]){
            ajaxDone["getPlaylists"] = 1;
            runAfterAjax();
        }
    });
}

function getSongList(id){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "get-data",
            data: "playlist",
            id: id,
        },
        type: "POST",
    }).done(function(result){
        if(result) {

            console.log("ASKING FOR PLAYLIST:"+id);
            
            $("#playlist-list ul li").removeClass("active"); // clear all the active classes from the platlists.

            $("#playlist-song-list ul li.song").remove(); // clear all the songs from the playlist

            var songs = JSON.parse(result);
            for (var i = 0; i < songs["songs"].length; i++) {

                var s = songs["songs"][i];

                var track        = s.track;
                var trackId      = s.trackid;
                var artist       = s.artist;
                var album        = s.album;
                var albumArtWork = s.albumartwork;
                var duration     = s.duration;

                var li = "\
                    <li id=\"track-" + trackId + "\" class=\"song\">\
                        <span class=\"glyphicon glyphicon-play btn btn-primary btn-sm song-play\"></span>\
                        <span class=\"song-name\">" + track + "</span>\
                        <span class=\"song-artist\">" + artist + "</span>\
                        <span class=\"song-album\">" + album + "</span>\
                        <span class=\"song-time\">" + duration / 1000 + "</span>\
                    </li>\
                ";

                $("#playlist-song-list ul").append(li);
            }

            id = id.replace(/:/g,''); // remove ":" from id
            // console.log($("#"+id));
            $("#"+id).addClass("active");
        }
    });
}

function getSongInfo(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "get-data",
            data: "now-playing"
        },
        type: "post",
    }).done(function(result){
        if(result) {

            $("#song-info div div").remove();

            var song = JSON.parse(result);

            var s = song["songs"][0];
            // var p = song["playlists"][0]; // undefined?

            var track        = s.track;
            var trackId      = s.trackid;
            var artist       = s.artist;
            var album        = s.album;
            var albumArtWork = s.albumartwork;
            var duration     = s.duration;

            $("#total-time").text(duration / 1000);

            var div = "\
                <div class=\"col-xs-5 col-sm-3 col-md-4 col-lg-2\">\
                    <img class=\"img-responsive song-img\" src=\"" + albumArtWork + "\" alt=\"Album Cover\"> \
                </div>\
                <div class=\"song-info-text\" id=\"" + trackId + "\">\
                    <p>" + track + "</p>\
                    <p>" + artist + "</p>\
                    <p>" + album + "</p>\
                </div>\
            ";

            $("#song-info div.row").append(div);
        }
        if(!ajaxDone["getSongInfo"]){
            ajaxDone["getSongInfo"] = 1;
            runAfterAjax();
        }
    });
}

function isPlaying(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "get-data",
            data: "is-playing"
        },
        type: "post",
    }).done(function(result){
        if(result == "1") { // if playing, show pause
            $("#song-control-toggle").addClass("glyphicon-pause");
            $("#song-control-toggle").removeClass("glyphicon-play");
        } else { // else, show play
            $("#song-control-toggle").removeClass("glyphicon-pause");
            $("#song-control-toggle").addClass("glyphicon-play");
        }
        if(!ajaxDone["isPlaying"]){
            ajaxDone["isPlaying"] = 1;
            runAfterAjax();
        }
    });
}

function getCurrentTime(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "get-data",
            data: "current-time"
        },
        type: "post",
    }).done(function(result){
        $("#current-time").text(result);
        console.log("TIME:"+toString(result));
        if(!ajaxDone["getCurrentTime"]){
            ajaxDone["getCurrentTime"] = 1;
            runAfterAjax();
        }
        if (result == "") {
            fakeResult += 100;
            percent = fakeResult / $("#total-time").text();
            $("#current-time").text(fakeResult / 1000);
        } else {
            percent = result / $("#total-time").text();
            $("#current-time").text(result / 1000);
            fakeResult = result; // set fakeresult incase of failure
        }
        console.log(percent);
        $(".progress-bar").css({"width": percent + "%"});
    });
}

//setInterval(function() {
//    getCurrentTime();
//}, 10000);

function playSong(songid, playlistid){
    console.log("PLAY SONG:"+songid+", ON:"+playlistid)
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "get-data",
            data: "song",
            songid: songid,
            playlistid: playlistid
        },
        type: "post",
    }).done(function(result){
        if (result == 1) {
            getSongInfo();
        }
    });
}

function changeSong(direction){
    console.log("CHANGE:"+direction);
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "play",
            data: direction,
        },
        type: "post",
    }).done(function(result){
        if (result == 1) {
            getSongInfo();
        }
    });
}

function search(term){
    console.log("SEARCH:"+term);
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "play",
            data: term,
        },
        type: "post",
    }).done(function(result){
        if (result == 1) {
            console.log("OK SEARCH RESPONSE");
        }
    });
}
