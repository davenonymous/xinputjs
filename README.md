> Node.js xinput wrapper. Access xinput based controllers

[![Build status](https://ci.appveyor.com/api/projects/status/1nxo39ety9q5itfs?svg=true)](https://ci.appveyor.com/project/thraaawn/xinputjs)

xinput.js wraps the xinput game controller API. Besides allowing raw access to the xinput device, it also provides a small wrapper class which emits events based on controller input.

## Contents

- [Requirements](#requirements)
- [Building](#building)
- [Examples](#examples)
- [License](#license)

## Requirements

[xinput](https://en.wikipedia.org/wiki/DirectInput#XInput) is only available on Windows. Please ensure you have the [required dependencies](https://github.com/nodejs/node-gyp#installation) as well as Visual Studio installed.

## Building

node-gyp is required to build xinput.js.

Install node-gyp using npm:

```
npm install -g node-gyp
```

Then configure and build:

```
node-gyp configure
node-gyp build
```

## Examples

### Simple
```JavaScript
var xinput = require('../');

[0, 1, 2, 3]
.filter(n => xinput.IsConnected(n))
.map(n => xinput.WrapController(n, {
	interval: 20,
	deadzone: {
		x: 0.20,
		y: 0.15
	},
	holdtime: 500
}))
.forEach(gamepad => {
	var n = gamepad.deviceNumber;

	gamepad.addListener("button-long", (button, elapsed) => {
		console.log("[%d] Hold button %s for %dms", n, button, elapsed);
		/* After holding the button for a while ->
		[1] Button buttons.a changed: true
		[1] Hold button buttons.a for 501ms
		[1] Button buttons.a changed: false
		*/
	});

	gamepad.addListener("button-short", (button, elapsed) => {
		console.log("[%d] Pressed button %s for %dms", n, button, elapsed);
		/* After tapping the button for a short moment ->
		[1] Button control.start changed: true
		[1] Button control.start changed: false
		[1] Pressed button control.start for 101ms
		*/
	});

	gamepad.addListener("button-changed", (button, state) => {
		console.log("[%d] Button %s changed: %s", n, button, state);
		/* Find example output in both listeners above */
	});

	gamepad.addListener("analog-input", (input, data) => {
		console.log("[%d] Holding %s at:", n, input, data);
		/* Moving the left stick to the left ->
		[1] Holding leftstick at: { x: -0.04042176580095827, y: 0 }
		[1] Holding leftstick at: { x: -0.17329172643208102, y: 0 }
		[1] Holding leftstick at: { x: -0.532532731101413, y: 0 }
		[1] Holding leftstick at: { x: -0.8130359813226723, y: 0 }
		[1] Holding leftstick at: { x: -0.9705114902188178, y: 0 }
		[1] Holding leftstick at: { x: -1, y: 0 }
		*/

		/* Holding both triggers for a short moment ->
		[1] Holding trigger at: { left: 0.2235294133424759, right: 0 }
		[1] Holding trigger at: { left: 0.658823549747467, right: 0.16078431904315948 }
		[1] Holding trigger at: { left: 1, right: 0.5098039507865906 }
		[1] Holding trigger at: { left: 1, right: 0.9960784316062927 }
		[1] Holding trigger at: { left: 1, right: 1 }
		[1] Holding trigger at: { left: 1, right: 1 }
		[1] Holding trigger at: { left: 1, right: 1 }
		[1] Holding trigger at: { left: 1, right: 1 }
		[1] Holding trigger at: { left: 1, right: 0.9960784316062927 }
		[1] Holding trigger at: { left: 0.4313725531101227, right: 0.37254902720451355 }
		*/
	});

	gamepad.addListener("connection-changed", (isConnected) => {
		console.log("[%d] Connection state changed: %s", n, isConnected ? "Connected!" : "Disconnected!");
		/* Pulling out the batteries + plugging them back in ->
		[1] Connection state changed: Disconnected!
		[1] Connection state changed: Connected!
		*/
	});
});
```

### Raw
```JavaScript
var xinput = require('../');

// Test all controllers 0-3
[0, 1, 2, 3].forEach(controllerNum => {
	console.log("Controller %d connected:", controllerNum, xinput.IsConnected(controllerNum));
	// -> Connected: true

	if(xinput.IsConnected(controllerNum)) {
		// Dump the current state
		console.log("Controller %d state:", controllerNum, xinput.GetState(controllerNum));
		/* -> {
		  // Digital inputs
		  buttons: { a: false, b: false, x: false, y: false },
		  dpad: { left: false, right: false, up: false, down: false },
		  shoulder: { left: false, right: false },
		  thumb: { left: false, right: false },
		  control: { back: false, start: false },

		  // Analog inputs
		  trigger: { left: 0, right: 0 },
		  leftstick: { x: -1679, y: 1514 },
		  rightstick: { x: -296, y: -3893 }
		} */

		// Vibrate and stop after 1s
		xinput.Vibrate(controllerNum, 0.5, 0.5);
		setTimeout(() => xinput.Vibrate(controllerNum, 0.0, 0.0), 1000);
	}
});
```

## License

MIT