
const link = document.createElement("link");
link.rel = "stylesheet";
link.href = "./lib/components.css";
document.head.appendChild(link);

$("led").each( function () {
    this.setColour = function (colour) {
        $(this).attr("colour", colour);
        $(this).css("backgroundColor", colour);
    }

    this.toggleOnOff = function () {
        state = !$(this).attr("state");
        $(this).attr("state", state)
        $(this).css("backgroundColor", state ? "white" : $(this).attr("colour") = colour);
    }
} );
