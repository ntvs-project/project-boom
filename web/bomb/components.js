
const _R = "red";
const _G = "green";
const _B = "blue";
const _W = "white";

const memFolourPatterns = [
    [
        [_G, _R, _B, _B, _W],
        [_R, _B, _R, _B, _G, _W],
        [_R, _R, _B, _G, _G, _W],
        [_B, _B, _R, _G, _W],
    ],
    [
        [_G, _R, _G, _B, _W],
        [_R, _B, _G, _W],
        [_G, _R, _B, _R, _G, _W],
        [_G, _B, _R, _W],
    ],
    [
        [_R, _G, _B, _G, _R, _W],
        [_B, _G, _R, _G, _G, _W],
        [_B, _R, _G, _B, _W],
        [_G, _R, _R, _B, _W],
    ],
    [
        [_R, _G, _B, _B, _W],
        [_R, _G, _B, _G, _B, _W],
        [_B, _R, _G, _R, _W],
        [_G, _G, _R, _G, _R, _W],
    ],
];

const memFolourAnswers = [
    [_R, _R, _B, _G],
    [_G, _R, _B, _G],
    [_R, _B, _R, _B],
    [_B, _G, _G, _R],
];

let YB = 0;

let pattern = memFolourPatterns[YB];
let answer  = memFolourAnswers[YB];
let user    = [0, 0, 0, 0];
let fixed   = [0, 0, 0, 0];
let toggles = [0, 0, 0, 0];
let index   = 0;

$("led").each(function (ledIdx) {
    this.idx = 0;
    this.interval = null;

    this.colourChange = () => {
        this.setColour(memFolourPatterns[YB][ledIdx][this.idx]);
        user[ledIdx] = memFolourPatterns[YB][ledIdx][this.idx];

        this.idx = (this.idx + 1) % memFolourPatterns[YB][ledIdx].length;
    }

    toggles[ledIdx] = () => {
        if (this.interval !== null) {
            clearInterval(this.interval);
            this.interval = null;
            return;
        }
        this.interval = setInterval(this.colourChange, 250);
    }
    toggles[ledIdx]();
});

$("btn").each(function () {
    $(this).on("click", function () {
        idx = $(this).attr("id").slice(3, 4) - 1;
        toggles[idx]();
        fixed[idx] = !fixed[idx];

        if (JSON.stringify(fixed) == JSON.stringify([true, true, true, true]) &&
            JSON.stringify(user)  == JSON.stringify(answer)) {
            alert("you win!");
        }
    });
});
