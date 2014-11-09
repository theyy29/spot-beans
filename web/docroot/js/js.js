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

    var desc = false;
    $(document).on("click", "#sort-song-name", function() {
        sortClass("song-name", desc);
        desc = !desc;
        return false;
    });

    getSongList();
    getPlaylists();
    getSongInfo();
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

    var sName  = [];
    var artist = [];
    var album  = [];
    var sTime  = [];

    for(var i = 0, l = $(".song-name").length; i < l; i++)
        sName.push($(".song-name")[i].innerHTML);

    for(var i = 0, l = $(".song-artist").length; i < l; i++)
        artist.push($(".song-artist")[i].innerHTML);

    for(var i = 0, l = $(".song-album").length; i < l; i++)
        album.push($(".song-album")[i].innerHTML);

    for(var i = 0, l = $(".song-time").length; i < l; i++)
        sTime.push($(".song-time")[i].innerHTML);

    for (var i = 0; i < sName.length; i++) {
        song.push({ "sName":sName[i], "artist":artist[i], "album":album[i], "sTime":sTime[i] });
    }

    for (var i = 0; i < song.length; i++) {
        console.log(song[i]);
    }

    song.sort(function(a, b) {
        return a.sName > b.sName;
    });

    for (var i = 0; i < song.length; i++) {
        console.log(song[i]);
    }

    sName  = [];
    artist = [];
    album  = [];
    sTime  = [];

    for (var i = 0; i < song.length; i++) {
         sName.push(song[i].sName);
         artist.push(song[i].artist);
         album.push(song[i].album);
         sTime.push(song[i].sTime);
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

    // for(var i = 0, l = song.length; i < l; i++)
    //     song[i].innerHTML = vals[i];
}
