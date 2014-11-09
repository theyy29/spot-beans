$(document).ready(function() {

    $(document).on("click", "#playlist-btn", function() {
        if ($("#playlist").hasClass("playlist-toggle")) {
            $("#overlay").hide();
            $("#playlist").removeClass("playlist-toggle");
        }
        else {
            $("#playlist").addClass("playlist-toggle");
            $("#overlay").show();
        }
    });

    $(document).on("click", "#overlay", function() {
        if ($("#playlist").hasClass("playlist-toggle"))
            $("#playlist").removeClass("playlist-toggle");
    });

    var descName = false;
    $(document).on("click", "#sort-song-name", function() {
        sortClass("song-name", descName);
        descName = !descName;
        return false;
    });

    var descArtist = false;
    $(document).on("click", "#sort-song-artist", function() {
        sortClass("song-artist", descName);
        descArtist = !descArtist;
        return false;
    });

    var descAlbum = false;
    $(document).on("click", "#sort-song-album", function() {
        sortClass("song-name", descAlbum);
        descAlbum = !descAlbum;
        return false;
    });

    var descTime = false;
    $(document).on("click", "#sort-song-time", function() {
        sortClass("song-name", descTime);
        descTime = !descTime;
        return false;
    });

    $(document).on("click", "#playlist-list ul li", function(){
        if (!($(this).hasClass("active"))){
            var id = $(this).attr('id');
            id = id.replace(/[^\d.]/g, ""); // remove everything but numbers
            // console.log(id)
            getSongList(id);
        }
    });

    getPlaylists();
    getSongInfo();
    isPlaying();

    if ($("#playlist-list ul li").hasClass("active")){
        var id = $(this).attr('id');
        id = id.replace(/[^\d.]/g, ""); // remove everything but numbers
        getSongList(id);
    }
});

/* http://stackoverflow.com/a/16983846 */
$(function () 
{
    $(".resizable1").resizable(
    {
        autoHide: true,
        handles: 'e',

        resize: function(e, ui) {

            var parent = ui.element.parent();
            var remainingSpace = parent.width() - ui.element.outerWidth(),
                divTwo = ui.element.next(),
                divTwoWidth = (remainingSpace - (divTwo.outerWidth() - divTwo.width()))/parent.width()*100+1+"%";
            // added 1px because of border ocd;
            if (remainingSpace > 400)
                divTwo.width(divTwoWidth);
        },

        stop: function(e, ui) {

            var parent = ui.element.parent();
            console.log(ui.element.width());

            if (ui.element.width() > 200)
                ui.element.css({
                    width: ui.element.width()/parent.width()*100+"%",
                });
        }
    });
});

function sortClass(classEle, sortDescending) {

    var song = [];

    var trackid   = [];
    var track     = [];
    var artist    = [];
    var album     = [];
    var duration  = [];

    $(".song").each(function() { 
        trackid.push($(this).attr("id").replace(/[^\d.]/g, ""))
    });

    for(var i = 0, l = $(".song-name").length; i < l; i++)
        track.push($(".song-name")[i].innerHTML);

    for(var i = 0, l = $(".song-artist").length; i < l; i++)
        artist.push($(".song-artist")[i].innerHTML);

    for(var i = 0, l = $(".song-album").length; i < l; i++)
        album.push($(".song-album")[i].innerHTML);

    for(var i = 0, l = $(".song-time").length; i < l; i++)
        duration.push($(".song-time")[i].innerHTML);

    for (var i = 0; i < track.length; i++) {
        song.push({ 
            "trackid":trackid[i],
            "track":track[i],
            "artist":artist[i],
            "album":album[i],
            "duration":duration[i]
        });
    }

    for (var i = 0; i < song.length; i++) {
        console.log(song[i]);
    }

    if (classEle.equals("song-name"))
        song.sort(function(a, b) {
            return a.track > b.track;
        });

    if (classEle.equals("song-artist"))
        song.sort(function(a, b) {
            return a.artist > b.artist;
        });

    if (classEle.equals("song-album"))
        song.sort(function(a, b) {
            return a.album > b.album;
        });

    if (classEle.equals("song-time"))
        song.sort(function(a, b) {
            return a.duration > b.duration;
        });

    for (var i = 0; i < song.length; i++) {
        console.log(song[i]);
    }

    track  = [];
    artist = [];
    album  = [];
    duration  = [];

    for (var i = 0; i < song.length; i++) {
         trackid.push(song[i].trackid);
         track.push(song[i].track);
         artist.push(song[i].artist);
         album.push(song[i].album);
         duration.push(song[i].duration);
    }    

    // for (var i = 0; i < song.length; i++) {
    //     console.log(song[i]);
    // }

    // var list = $("."+classEle);
    // var vals = [];

    // for(var i = 0, l = list.length; i < l; i++)
    //     vals.push(list[i].innerHTML);

    // vals.sort();

    // if(sortDescending)
    //     vals.reverse();

    // for(var i = 0, l = list.length; i < l; i++)
    //     list[i].innerHTML = vals[i];

    if(sortDescending)
        song.reverse();

    // change the list of the page
    // for(var i = 0, l = song.length; i < l; i++)
    //     song[i].innerHTML = vals[i];

    $("#playlist-song-list ul li.song").remove(); // clear all the songs from the playlist

    for (var i = 0; i < song.length; i++) {
        var s = song[i];
        var li = "\
            <li id=\"track-" + s.trackid + "\" class=\"song\">\
                <span class=\"glyphicon glyphicon-play btn btn-primary btn-sm song-play\"></span>\
                <span class=\"song-name\">" + s.track + "</span>\
                <span class=\"song-artist\">" + s.artist + "</span>\
                <span class=\"song-album\">" + s.album + "</span>\
                <span class=\"song-time\">" + s.duration + "</span>\
            </li>\
        ";
        $("#playlist-song-list ul").append(li);
    }
}
