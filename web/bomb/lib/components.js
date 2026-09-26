
document.addEventListener("contextmenu", e => e.preventDefault());

// led
$("led").each( function () {
    this.setColour = function (colour) {
        $(this).attr("colour", colour);
        if ($(this).attr("state") == "0" || colour == "transparent") this.setOFF(true);
        else this.setON(true);
    }

    this.setSize = function (size="60") {
        $(this).attr("size", size);
        $(this).css("width",  `${size}px`);
        $(this).css("height", `${size}px`);
    }
    
    this.setON = function () {
        $(this).attr("state", 1);
        $(this).css("background-color", $(this).attr("colour"));
        $(this).css("border", "");
    }
    
    this.setOFF = function () {
        $(this).attr("state", 0);
        $(this).css("background-color", "transparent");
        $(this).css("border", "2px solid black");
    }

    this.setState = function (state) {
        if (state == 0) this.setOFF();
        else this.setON();
    }

    if ($(this).attr("state") == 0) this.setOFF();
    else this.setON();
    this.setColour($(this).attr("colour"));
    this.setSize($(this).attr("size"));
} );

// 7seg
const segmentsNumber = [
    "1111110",
    "0110000",
    "1101101",
    "1111001",
    "0110011",
    "1011011",
    "1011111",
    "1110000",
    "1111111",
    "1111011",
];

function initSegmentParts() {
    $("seg7a, seg7b, seg7c, seg7d, seg7e, seg7f, seg7g, seg7p, seg7dot").each( function () {
        this.setColour = function (colour) {
            $(this).attr("colour", colour);
            if ($(this).attr("state") == "0" || colour == "transparent") this.setOFF(true);
            else this.setON(true);
        }
        
        this.setON = function () {
            $(this).attr("state", 1);
            $(this).css("background-color", $(this).attr("colour"));
        }
        
        this.setOFF = function () {
            $(this).attr("state", 0);
            $(this).css("background-color", "transparent");
        }

        if ($(this).attr("state") == 0) this.setOFF();
        else this.setON();
        this.setColour($(this).attr("colour"));
    } );

    $("seg7colon").each( function () {
        this.setColour = function (colour="red") {
            $(this).attr("colour", colour);
            $(this).children().each( function () {
                this.setColour(colour);
            } );
        }
        
        this.setON = function () {
            $(this).attr("state", 1);
            $(this).children().each( function () {
                this.setON();
            } );
        }
        
        this.setOFF = function () {
            $(this).attr("state", 0);
            $(this).children().each( function () {
                this.setOFF();
            } );
        }

        if ($(this).attr("state") == 0) this.setOFF();
        else this.setON();
        this.setColour($(this).attr("colour"));
    } );
};

function init7Segments () {
    $("seg7").each( function () {
        this.setColour = function (colour="red") {
            $(this).attr("colour", colour);
            $(this).children().each( function () {
                this.setColour(colour);
            } );
        }

        this.setSize = function (size="5") {
            $(this).attr("size", size);
            $(this).css("--width",   `${size}px`);
            $(this).css("--length",  `${4 * size}px`);
            $(this).css("--padding", `${2 * size}px`);
        }

        this.setSegment = function (seg) {
            seg = seg.padEnd(8, "0")
            $(this).attr("seg", seg);
            $(this).html(`
                <seg7a state="${seg[0]}"></seg7a>
                <seg7b state="${seg[1]}"></seg7b>
                <seg7c state="${seg[2]}"></seg7c>
                <seg7d state="${seg[3]}"></seg7d>
                <seg7e state="${seg[4]}"></seg7e>
                <seg7f state="${seg[5]}"></seg7f>
                <seg7g state="${seg[6]}"></seg7g>
                <seg7p state="${seg[7]}"></seg7p>
            `);
            initSegmentParts();
        };

        let $seg = ($(this).attr("seg") || "").replace("_", "");
        let $num = $(this).attr("num");
        if ($num) $seg = segmentsNumber[+$num] + ($num[1] == "." ? "1" : "0");
        $(this).text("");
        this.setSegment($seg);
        this.setSize($(this).attr("size"));
        this.setColour($(this).attr("colour"));
    } );
}

init7Segments();

// quad 7seg
$("seg7quad").each( function () {
    this.setColour = function (colour="red") {
        $(this).attr("colour", colour);
        $(this).children().each( function () {
            this.setColour(colour);
        } );
    }

    this.setSize = function (size="5") {
        $(this).attr("size", size);
        $(this).children().css("--width",     `${size}px`);
        $(this).find("seg7").css("--length",  `${4 * size}px`);
        $(this).find("seg7").css("--padding", `${2 * size}px`);
    }

    this.setNumber = function (num) {
        num = `${num}`;
        $(this).html(`
            <seg7 num="${num[0]}"></seg7>
			<seg7 num="${num[1]}"></seg7>
			<seg7colon state="1">
				<seg7dot></seg7dot>
				<seg7dot></seg7dot>
			</seg7colon>
			<seg7 num="${num[2]}"></seg7>
			<seg7 num="${num[3]}"></seg7>
        `);
        init7Segments();
    }

    $(this).text("");
    this.setNumber($(this).attr("num"));
    this.setSize($(this).attr("size"));
    this.setColour($(this).attr("colour"));
} );

// button
$("btn").each( function () {
    this.setColour = function (colour) {
        $(this).attr("colour", colour);
        $(this).css("background-color", colour);
    }

    this.setSize = function (size="60") {
        $(this).attr("size", size);
        $(this).css("width",  `${size}px`);
        $(this).css("height", `${size}px`);
    }

    const holdDuration = 500;
    let holdTimer;
    let holdFired = false;

    $(this).on("pointerdown", function (e) {
        e.preventDefault();

        holdFired = false;
        clearTimeout(holdTimer);

        holdTimer = setTimeout(() => {
            holdFired = true;
            $(this).trigger("hold");
        }, holdDuration);
    });

    $(this).on("pointerup pointercancel pointerleave", function (e) {
        e.preventDefault();

        clearTimeout(holdTimer);

        if (!holdFired && e.type === "pointerup") {
            $(this).trigger("pressed");
        }
    });

    this.setColour($(this).attr("colour"));
    this.setSize($(this).attr("size"));
} );

// matrix
$("matrix").each( function () {
    this.setColour = function (colour) {
        $(this).attr("colour", colour);
        $(this).css("background-color", colour);
    }

    this.setSize = function (size="0.8") {
        $(this).attr("size", size);
        $(this).css("zoom",  `${size}`);
        $(this).css("border-width",  `${2 / size}px`);

        $(this).find("btn").each( function () {
            $(this).css("border-width",  `${2 / size}px`);
            $(this).css("font-weight",  `${400 / size}`);
        } );
    }

    $(this).css("grid-template-columns", `repeat(${$(this).attr("col")}, max-content)`);
    $(this).css("grid-template-rows", `repeat(${$(this).attr("row")}, max-content)`);

    this.setColour($(this).attr("colour"));
    this.setSize($(this).attr("size"));
} );

// knob
$("knob").each( function () {
    let max = $(this).attr("max");
    let min = $(this).attr("min");
    let range = max - min + 1;
    const sensitivity = 5;

    let count = 0;
    let dragging = false;
    let startY = 0;
    let startCount = 0;

    let $this = $(this);

    $(this).on("mousedown touchstart", function (e) {
        dragging = true;
        startY = e.type === "touchstart" ? e.originalEvent.touches[0].clientY : e.clientY;
        startCount = count;
    });

    $(document).on("mousemove touchmove", function (e) {
        if (!dragging) return;
        const currentY = e.type === "touchmove" ? e.originalEvent.touches[0].clientY : e.clientY;
        count = startCount + Math.round((startY - currentY) / sensitivity);
        count = Math.min(Math.max(min, count), max);
        $this.css("rotate", `${ 340 * count / range - 170 }deg`);
    });

    $(document).on("mouseup touchend", function () {
        dragging = false;
    });

    $(this).on("dblclick", function () {
        count = 0;
        $this.css("rotate", `${ 340 * count / range - 170 }deg`);
    });
} );

// buzzer
var ctx = null;
function beep(freq, ms, wait) {
  if (wait === undefined) wait = 50;

  if (!ctx) ctx = new (window.AudioContext || window.webkitAudioContext)();
  ctx.resume();

  var osc = ctx.createOscillator();
  var gain = ctx.createGain();
  osc.type = "square";
  osc.frequency.value = freq;
  gain.gain.value = 0.05; // volume
  osc.connect(gain);
  gain.connect(ctx.destination);
  osc.start();
  osc.stop(ctx.currentTime + ms / 1000);

  return new Promise(function (done) { setTimeout(done, ms + wait); });
}
