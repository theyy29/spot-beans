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
});