
if (localStorage.start == undefined) {
    $("#grid > div > a:not(#mainboard > a)").each( function () {
        $(this).parent().html(
            $(this).html()
        );
    } );
}
