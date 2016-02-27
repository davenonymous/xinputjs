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