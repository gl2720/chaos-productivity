var interval = 25 * 60 * 1000
const timer = {
  id: undefined,
  timeLeft: 0,
  timeTotal: 0
};
var port;

// Receives messages from popup:
// start_timer: starts timer, no return
// stop_timer: stops timer, no return
// get_time: returns current time to popup, or no_time if no timer is running
// hi!: returns welcome message to popup

// Sends messages to popup:
// a time/no_time: in response to get_time
// timer_end: when timer ends

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
        startTimer();
      } else if (request.message === 'stop_timer' && timer.id) {
        stopTimer();
      } else if (request.message === 'get_time') {
        if (timer.id) {
          let seconds = timer.timeLeft;
          port.postMessage({message: `${Math.floor(seconds / 60)}`.padStart(2, '0') + ':' + `${seconds % 60}`.padStart(2, '0')}); //
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
  completedTime = timer.timeTotal - timer.timeLeft;
  timer.id = clearTimeout(timer.id);
}

function timerEnd() {
  completedTime = timer.timeTotal;
  timer.id = clearTimeout(timer.id);
  chrome.notifications.create({ // notification gets handled by MacOS
    type: 'basic',
    iconUrl: 'milky-way_1f30c.png',
    title: 'Time\'s up!',
    message: 'Take a break!',
    buttons: [{title: 'Dismiss'}, {title: 'Start Break'}]
  });
}

function updateTime(seconds) {
  // format time as mm:ss
  text = `${Math.floor(seconds / 60)}`.padStart(2, '0') + ':' + `${seconds % 60}`.padStart(2, '0');
  if (!timer.id) timer.timeTotal = seconds;
  timer.timeLeft = seconds;
  if (port) port.postMessage({message: text});
  chrome.action.setBadgeText({text: text});
  // time tracking
  if (seconds > 0) {
    timer.id = setTimeout(() => updateTime(seconds - 1), 1000);
  } else {
    if (port) port.postMessage({message: 'timer_end'});
    timerEnd();
  }
}