
const morseCode = [
  "01--", "1000", "1010", "100-", "0---", "0010", "110-",
  "0000", "00--", "0111", "101-", "0100", "11--", "10--",
  "111-", "0110", "1101", "010-", "000-", "1---", "001-",
  "0001", "011-", "1001", "1011", "1100"
];

const morseLength = [
  2, 4, 4, 3, 1, 4, 3,
  4, 2, 4, 3, 4, 2, 2,
  3, 4, 4, 3, 3, 1, 3,
  4, 3, 4, 4, 4
];

function toBinary (number, padding=4) {
    return number.toString(2).padStart(padding, "0");
}

function binaryToGrey (binary) {
    return binary ^ (binary >> 1);
}

function greyToBinary (grey) {
    let binary = grey;
    while (grey >>= 1) {
        binary ^= grey;
    }
    return binary;
}

function leftShift (original, shiftAmount) {
    return (original > shiftAmount) ? (original - shiftAmount) : (original - shiftAmount + 26);
}

function rightShift (original, shiftAmount) {
    return (original + shiftAmount < 26) ? (original + shiftAmount) : (original + shiftAmount - 26);
}

const YB = Math.floor(Math.random() * 4);
const Y  = YB >> 1;
const B  = YB % 2;
$("#ledY")[0].setState(Y);
$("#ledB")[0].setState(B);

const isGrey  = (Y != B);
const shiftAmount = Math.floor(Math.random() * 8);
const cipher  = Math.floor(Math.random() * (isGrey ? 16 : 26));
const answer  = isGrey
                ? (Y == 1 ? binaryToGrey(cipher) : greyToBinary(cipher))
                : (Y == 0 ? leftShift(cipher, shiftAmount) : rightShift(cipher, shiftAmount));
let user      = ["-", "-", "-", "-"];
let inputIdx  = 0;

function check () {
    if (isGrey) {
        return ( toBinary(answer) == user.join("") );
    } else {
        return ( morseCode[answer] == user.join("") );
    }
}

function setup() {
    $("main led").each( function (idx) {
        if (isGrey) {
            this.setColour("white")
            this.setON();
        } else {
            if (morseCode[cipher].split("")[idx] == "-") {
                this.setOFF();
            } else {
                this.setColour("white");
                this.setON();
            }
        }
    });
}

function update() {
    $("main led")[inputIdx].setColour("cyan");
    $("main led")[inputIdx].setON();
    inputIdx++;

    if (inputIdx == (isGrey ? 4 : morseLength[answer])) {
        if (check()) {
            alert("you win!");
            location.href = location.href;
        } else {
            inputIdx = 0;
            user = ["-", "-", "-", "-"];

            setup();
        }
    }
}

$("#btn").on("pressed", () => {
    console.log(inputIdx);
    user[inputIdx] = 0;
    
    update();
});

$("#btn").on("hold", () => {
    console.log(inputIdx);
    user[inputIdx] = 1;
    
    update();
});

async function main () {
    await beep(0, 200);
    while (1) {
        if (isGrey) {
            await beep(700, +toBinary(cipher)[0] * 600 + 300, 100);
            await beep(700, +toBinary(cipher)[1] * 600 + 300, 100);
            await beep(700, +toBinary(cipher)[2] * 600 + 300, 100);
            await beep(700, +toBinary(cipher)[3] * 600 + 300, 100);

            await new Promise(r => setTimeout(r, 300));
        } else {
            for (const digit of morseCode[cipher].split("")) {
                if (digit == "0") await beep(1000, 300, 100);
                if (digit == "1") await beep(1000, 900, 100);
            }

            await new Promise(r => setTimeout(r, 300));

            for (const digit of toBinary(shiftAmount, 3).split("")) {
                await beep(500, digit === "0" ? 300 : 900, 100);
            }

            await new Promise(r => setTimeout(r, 300));
        }
    }
}

setup();
$("#start").on("click", function () {
    $(this).remove();
    main();
});
