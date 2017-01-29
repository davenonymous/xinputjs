"use strict";

var xinput = require('bindings')('xinput');

const EventEmitter = require('events');
class XInputDevice extends EventEmitter {
	constructor(deviceNumber, options) {
		super();

		this.options = {
			interval: 20,
			deadzone: {
				x: 0.20,
				y: 0.15
			},
			holdtime: 500
		};

		if(options) {
			Object.keys(options).forEach(option => this.setOption(option, options[option]));
		}

		this.deviceNumber = deviceNumber;
		this.connected = xinput.IsConnected(this.deviceNumber);
		this.state = xinput.GetState(this.deviceNumber);

		this._previousState = this.state;
		this._digitalStates = ['buttons', 'dpad', 'shoulder', 'thumb', 'control'];
		this._analogStates = ['trigger', 'leftstick', 'rightstick'];
		this._holdTimes = {};
		this._holdTimers = {};

		this.initLoop();
	}

	setOption(key, value) {
		this.options[key] = value;
	}

	stopLoop() {
		if(this.eventLoop == null) {
			return;
		}

		clearInterval(this.eventLoop);
	}

	initLoop() {
		this.stopLoop();
		this.eventLoop = setInterval(this._update.bind(this), this.options.interval);
	}

	getNormalizedState(section, button) {
		return this._normalize(section, button, this.state[section][button]);
	}

	_normalize(section, button, val) {
		if(section == "leftstick" || section == "rightstick") {
			let normed = Math.max(-1, val / 32767);
			let zoned = (Math.abs(normed) < this.options.deadzone[button] ? 0 : (Math.abs(normed) - this.options.deadzone[button]) * (normed / Math.abs(normed)));

			if (this.options.deadzone[button] > 0) zoned /= 1 - this.options.deadzone[button];

			return zoned;
		}

		return val;
	}

	_onConnectedChanged(isConnected) {
		this.emit("connection-changed", isConnected);
		this.emit("connection-" + isConnected ? "established" : "closed");
	}

	_onInputChanged(section, button, state) {
		let key = `${section}.${button}`;
		this.emit("_input-changed", key, state);

		if(this._digitalStates.indexOf(section) != -1) {
			this.emit("button-changed", key, state);

			if(state) {
				this._holdTimes[key] = new Date().getTime();
				this._holdTimers[key] = setTimeout((() => {
					let now = new Date().getTime();
					let timePressed = this._holdTimes[key] || now;
					let elapsed = now - timePressed;

					this.emit("button-long", key, elapsed);
				}).bind(this), this.options.holdtime);
			} else {
				let now = new Date().getTime();
				let timePressed = this._holdTimes[key] || now;
				let elapsed = now - timePressed;

				clearTimeout(this._holdTimers[key]);

				if(elapsed < this.options.holdtime) {
					this.emit("button-short", key, elapsed);
				}
			}
		}
	}

	_update() {
		let state = xinput.GetState(this.deviceNumber);
		let connected = xinput.IsConnected(this.deviceNumber);

		this.state = state;

		if(this.connected != connected) {
			this._onConnectedChanged(connected);
			this.connected = connected;
		}

		Object.keys(state).forEach(section => {
			Object.keys(state[section]).forEach(button => {
				let value = this._normalize(section, button, state[section][button]);
				let previous = this._normalize(section, button, this._previousState[section][button]);

				if(previous != value) {
					this._onInputChanged(section, button, value);
				}
			});
		});

		this._analogStates.forEach(section => {
			let hasInput = false;
			let values = {};

			Object.keys(state[section]).forEach(button => {
				let value = this._normalize(section, button, state[section][button]);
				if(value != 0) {
					hasInput = true;
				}
				values[button] = value;
			});

			if(hasInput) {
				this.emit("analog-input", section, values);
			}
		});

		this._previousState = state;
	}
}

xinput.WrapController = function(deviceNumber, options) {
	return new XInputDevice(deviceNumber, options);
};

module.exports = xinput;