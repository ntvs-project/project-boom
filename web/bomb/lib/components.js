
// led
$("led").each( function () {
    this.setON = function (inner=false) {
        if (!inner) $(this).attr("state", 1);
        $(this).css("backgroundColor", $(this).attr("colour"));
        $(this).css("border", "");
    }
    
    this.setOFF = function (inner=false) {
        if (!inner) $(this).attr("state", 0);
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
        if ($(this).attr("state") == "0" || colour == "transparent") this.setOFF(true);
        else this.setON(true);
    }

    this.setColour($(this).attr("colour"));

    state = $(this).attr("state");
    if (state == undefined) this.setState(1);
    if (state == 1) this.setON();
    if (state == 0) this.setOFF();
} );

// button
$("btn").each(function () {
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
});

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
