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

$(function () 
{
    $(".resizable1").resizable(
    {
        autoHide: true,
        handles: 'e',
        resize: function(e, ui) 
        {
            var parent = ui.element.parent();
            var remainingSpace = parent.width() - ui.element.outerWidth(),
                divTwo = ui.element.next(),
                divTwoWidth = (remainingSpace - (divTwo.outerWidth() - divTwo.width()))/parent.width()*100+20+"%";
                divTwo.width(divTwoWidth);
        },
        stop: function(e, ui) 
        {
            var parent = ui.element.parent();
            ui.element.css(
            {
                width: ui.element.width()/parent.width()*100+"%",
            });
        }
    });
});
