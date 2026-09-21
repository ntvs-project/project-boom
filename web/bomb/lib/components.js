
$("led").each( function () {
    this.setON = function () {
        $(this).attr("state", 1);
        $(this).css("backgroundColor", $(this).attr("colour"));
        $(this).css("border", "");
    }
    
    this.setOFF = function () {
        $(this).attr("state", 0);
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
        if ($(this).attr("state") == "1") $(this).css("backgroundColor", colour);
    }

    this.setColour($(this).attr("colour"));

    state = $(this).attr("state");
    if (state == undefined) this.setState(1);
    if (state == 1) this.setON();
    if (state == 0) this.setOFF();
} );

var ctx = null;
function beep(freq, ms, wait) {
  if (wait === undefined) wait = 50;

  if (!ctx) ctx = new (window.AudioContext || window.webkitAudioContext)();
  ctx.resume();

  var osc = ctx.createOscillator();
  var gain = ctx.createGain();
  osc.type = "square";
  osc.frequency.value = freq;
  gain.gain.value = 0.15;
  osc.connect(gain);
  gain.connect(ctx.destination);
  osc.start();
  osc.stop(ctx.currentTime + ms / 1000);

  return new Promise(function (done) { setTimeout(done, ms + wait); });
}
