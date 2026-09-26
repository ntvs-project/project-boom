
$("#btn1").on("click", function () {
    if ($("#led1").attr("colour") == "magenta") {
        $("#led1")[0].setColour("orange");
    } else {
        $("#led1")[0].setColour("magenta");
    }
});

$("#btn2")[0].setColour("red");
$("#btn2").on("click", function () {
    if ($("#led2").attr("state") == 1) {
        $("#led2")[0].setOFF();
    } else {
        $("#led2")[0].setON();
    }
});

$("#btn3").on("click", async function () {
    const dih = 100;
    const dah = dih * 3;

    await beep(700, dah);
    await beep(700, dih);
    await beep(700, dah);
    await beep(700, dah);
} );

$("#btn4").on("click", async function () {
    const dih = 100;
    const dah = dih * 3;

    await beep(1000, dah);
    await beep(1000, dih);
    await beep(1000, dah);
    await beep(1000, dih, dah);
    await beep(500, dah);
    await beep(500, dah);
    await beep(500, dah);
} );

$("matrix")[0].setColour("#F5DEB3");
$("matrix btn").each( function () { this.setColour("#FAEBD7"); } )
$("matrix btn").on("click", function () { alert(`button "${ $(this).attr("value") }" is clicked`); } );
