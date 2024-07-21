// var creates global property
var port = chrome.runtime.connect({name: 'popup'}); // connect to background.js
port.postMessage({message: 'get_time'});
port.onMessage.addListener((response) => {
  msg = response.message;
  if (msg === 'no_time') {
    setupButton('Start', startTimer);
    chrome.action.setBadgeText({text: ''});
  } else if (msg === 'timer_end') {
    setupButton('Restart', startTimer, stopTimer);
    chrome.action.setBadgeText({text: ''});
  } else {
    setupButton('Stop', stopTimer, startTimer);
    document.getElementById('time').innerHTML = msg;
    // ideally the badge might not exist when the popup is open but then I would need to put the badge back up upon popup close
  }
});

function setupButton(text, eventToAdd, eventToRemove) {
  if (eventToRemove) document.getElementById('startButton').removeEventListener('click', eventToRemove);
  document.getElementById('startButton').textContent = text;
  document.getElementById('startButton').addEventListener('click', eventToAdd);
}

function startTimer() {
  port.postMessage({message: 'start_timer'});
  setupButton('Stop', stopTimer, startTimer);
}

function stopTimer() {
  port.postMessage({message: 'stop_timer'});
  setupButton('Restart', startTimer, stopTimer);
  chrome.action.setBadgeText({text: ''});
}