
const link = document.createElement("link");
link.rel = "stylesheet";
link.href = "./lib/components.css";
document.head.appendChild(link);

$("led").each( function () {
    this.setON = function () {
        $(this).attr("state", 1);
        $(this).css("backgroundColor", $(this).attr("colour"));
        $(this).css("border", "");
    }
    
    this.setOFF = function () {
        $(this).attr("state", 0);
        $(this).css("backgroundColor", "transparent");
        $(this).css("border", "2px solid black");
    }

    this.setState = function (state) {
        $(this).attr("state", state);
        if (state == 1) this.setON();
        if (state == 0) this.setOFF();
    }

    this.setColour = function (colour) {
        $(this).attr("colour", colour);
        if ($(this).attr("state") == "1") $(this).css("backgroundColor", colour);
    }

    this.setColour($(this).attr("colour"));

    state = $(this).attr("state");
    if (state == undefined) this.setState(1);
    if (state == 1) this.setON();
    if (state == 0) this.setOFF();
} );
