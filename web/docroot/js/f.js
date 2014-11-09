function togglePlayPause(){
    $.ajax({
        url: "ajax.php",
        data: {
            operation: "toggle-playing"
        },
        type: "POST",
    }).done(function(result){
        alert("RESULT");
        if(result == "1")
            // TOGGLE THE IMAGE or class or whatever
            ;
    });
}
