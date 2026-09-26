
let YB = Math.floor(Math.random() * 4);
$("#ledY")[0].setState(YB >> 1);
$("#ledB")[0].setState(YB % 2);

const answer = [
    "0454", "0316", "0268", "2502"
];

let segments;
let user    = ["", "", "", ""];
let inputIdx   = 0;

const $first  = $("[id*=seg1]");
const $second = $("[id*=seg2]");

const segNumber = [
    "1111110", // 0
    "0110000", // 1
    "1101101", // 2
    "1111001", // 3
    "0110011", // 4
    "1011011", // 5
    "1011111", // 6
    "1110000", // 7
    "1111111", // 8
    "1111011", // 9
];

const segSin = ["1011011", "0110000", "0010101"];
const segCos = ["1001110", "0011101", "1011011"];
const segTan = ["0001111", "1110111", "0010101"];
const segLog = ["0001110", "0011101", "1111011"];
const segDeg = "1100011";

function setup() {
    switch (YB) {
        case 0:
            segments = [...segSin, segNumber[2], segNumber[7], segDeg];
            break;
        case 1:
            segments = [...segCos, segNumber[7], segNumber[1], segDeg];
            break;
        case 2:
            segments = [...segTan, segNumber[1], segNumber[5], segDeg];
            break;
        case 3:
            segments = [...segLog, segNumber[3], segNumber[1], segNumber[8]];
            break;
        default:
            break;
    }

    $first.each( function (idx) {
        this.setSegment(segments[idx]);
    } );

    $second[0].setSegment("00000001");
    $second[1].setSegment("");
    $second[2].setSegment("");
    $second[3].setSegment("");
}

async function check () {
    return (user.join("") == answer[YB]);
}

function update() {
    $second[inputIdx].setSegment(segNumber[user[inputIdx]] + (inputIdx == 0 ? "1" : "0"));
    inputIdx++;

    if (inputIdx == 4) {
        if (check()) {
            alert("you win!");
            location.href = "/";
        } else {
            inputIdx = 0;
            user = ["", "", "", ""];

            setup();
        }
    }
}

$("matrix btn").each(function (idx) {
    $(this).on("click", () => {
        value = $(this).attr("value");
        if (["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"].includes(value)) {
            user[inputIdx] = value;
            update();
        }

    });
});

setup();
