
function getDifference () {
    return 601 + Math.floor(moment(localStorage.start).diff(moment()) / 1000);
}

function update () {
    $("#ledY")[0].setState(+localStorage.YB >> 1);
    $("#ledB")[0].setState(+localStorage.YB % 2);

    $("seg7quad")[0].setNumber( moment(getDifference() * 1000).format("mmss") );
    setInterval(() => {
        $("seg7quad")[0].setNumber( moment(getDifference() * 1000).format("mmss") );

        if (getDifference() < 0) {
            localStorage.clear();
            alert("time's up");
            location.href = location.href;
        }
    }, 1000);
}

$("#btn-reset").on("click", function () {
    localStorage.clear();
    location.href = location.href;
} );

$("#btn-start").on("click", function () {
    if (localStorage.start == undefined) {
        localStorage.start = moment().format();
        localStorage.YB = Math.floor(Math.random() * 4);
        localStorage.miss = 0;
        localStorage.finished = 0;
    }
    update();
} );

if (localStorage.start != undefined) {
    update();
}
