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
