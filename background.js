var interval = 25 * 60 * 1000
var countdown;
var port;

// receives messages from popup (start time, stop time, get time)
chrome.runtime.onConnect.addListener(listener);

function listener(port) {
  // console.assert(port.name === "popup");
  console.log('connected', port);
  this.port = port;
  port.onDisconnect.addListener(() => {
    this.port = undefined;
    console.log('disconnected');
  });

  if (port.name === 'popup') {
    port.onMessage.addListener((request) => {
      if (request.message === 'start_timer') {
        console.log(countdown);
        startTimer();
      } else if (request.message === 'stop_timer' && countdown) {
        stopTimer();
      } else if (request.message === 'get_time') {
        if (countdown) {
          port.postMessage({message: 'a time'}); //
        } else {
          port.postMessage({message: 'no_time'});
        }
      } else if (request.message === 'hi!') {
          console.log('hi from popup!');
          port.postMessage({message: 'welcome home!'});
      }
  });
  }
}

function startTimer() {
  // start counting, update every 1 second
  let seconds = interval / 1000;
  updateTime(seconds);
}

function stopTimer() {
  countdown = clearTimeout(countdown); // or clearInterval whatever
}

function updateTime(seconds) { // as opposed to setInterval
  // format time as mm:ss
  text = `${Math.floor(seconds / 60)}`.padStart(2, '0') + ':' + `${seconds % 60}`.padStart(2, '0');
  console.log(text);
  if (port) port.postMessage({message: text});
  chrome.action.setBadgeText({text: text}); // persists even if popup is closed/stopped
  // time tracking
  if (seconds > 0) {
    countdown = setTimeout(() => updateTime(seconds - 1), 1000);
  } else {
    console.log("Time's up!");
    stopTimer();
  }
}