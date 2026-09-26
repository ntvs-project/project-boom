
const _R = "red";
const _G = "green";
const _B = "blue";
const _T = "transparent";

const memFolourPatterns = [
    [
        [_G, _R, _B, _B, _T],
        [_R, _B, _R, _B, _G, _T],
        [_R, _R, _B, _G, _G, _T],
        [_B, _B, _R, _G, _T],
    ],
    [
        [_G, _R, _G, _B, _T],
        [_R, _B, _G, _T],
        [_G, _R, _B, _R, _G, _T],
        [_G, _B, _R, _T],
    ],
    [
        [_R, _G, _B, _G, _R, _T],
        [_B, _G, _R, _G, _G, _T],
        [_B, _R, _G, _B, _T],
        [_G, _R, _R, _B, _T],
    ],
    [
        [_R, _G, _B, _B, _T],
        [_R, _G, _B, _G, _B, _T],
        [_B, _R, _G, _R, _T],
        [_G, _G, _R, _G, _R, _T],
    ],
];

const memFolourAnswers = [
    [_R, _R, _B, _G],
    [_G, _R, _B, _G],
    [_R, _B, _R, _B],
    [_B, _G, _G, _R],
];

let YB = Math.floor(Math.random() * 4);
$("#ledY")[0].setState(YB >> 1);
$("#ledB")[0].setState(YB % 2);

let patterns = memFolourPatterns[YB];
let answer   = memFolourAnswers[YB];
let user    = [0, 0, 0, 0];
let toggles = [0, 0, 0, 0];
let fixed   = 0;

$("main led").each(function (ledIdx) {
    this.patternIdx = 0;
    this.interval = null;

    this.colourChange = () => {
        pattern = patterns[ledIdx][this.patternIdx];
        user[ledIdx] = pattern;
        this.setColour(pattern);

        this.patternIdx = (this.patternIdx + 1) % patterns[ledIdx].length;
    }

    toggles[ledIdx] = () => {
        if (this.interval !== null) {
            fixed += 1;
            clearInterval(this.interval);
            this.interval = null;
            return;
        }
        fixed -= 1;
        this.interval = setInterval(this.colourChange, 250);
    }

    toggles[ledIdx]();
    fixed = 0;
});

function check () {
    return (JSON.stringify(fixed) == 4 && JSON.stringify(user)  == JSON.stringify(answer))
}

$("main btn").each(function (idx) {
    $(this).on("click", () => {
        toggles[idx]();
        fixed[idx] = !fixed[idx];

        if (check()) {
            alert("you win!");
            location.href = "/";
        }
    });
});
