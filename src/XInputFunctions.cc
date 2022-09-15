#include "XInputFunctions.h"
#include <XInput.h>

NAN_METHOD(Vibrate) {
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	double left = info[1]->IsUndefined() ? 0 : Nan::To<double>(info[1]).FromJust();
	double right = info[2]->IsUndefined() ? 0 : Nan::To<double>(info[2]).FromJust();

	vibration.wLeftMotorSpeed = (WORD)(65535 * left);
	vibration.wRightMotorSpeed = (WORD)(65535 * right);

	// TODO: Verify parameters
	XInputSetState((DWORD)info[0]->Uint32Value(Nan::GetCurrentContext()).ToChecked(), &vibration);
}

NAN_METHOD(IsConnected) {
	XINPUT_STATE controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	// TODO: Verify parameters
	DWORD result = XInputGetState((DWORD)info[0]->Uint32Value(Nan::GetCurrentContext()).ToChecked(), &controllerState);
	if(result == ERROR_SUCCESS) {
		info.GetReturnValue().Set(true);
	} else {
		info.GetReturnValue().Set(false);
	}
}

NAN_METHOD(GetState) {
	XINPUT_STATE controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	// TODO: Verify parameters
	DWORD result = XInputGetState((DWORD)info[0]->Uint32Value(Nan::GetCurrentContext()).ToChecked(), &controllerState);


	v8::Local<v8::Object> objButtons = Nan::New<v8::Object>();
	objButtons->Set(Nan::GetCurrentContext(),Nan::New("a").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0));
	objButtons->Set(Nan::GetCurrentContext(),Nan::New("b").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0));
	objButtons->Set(Nan::GetCurrentContext(),Nan::New("x").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0));
	objButtons->Set(Nan::GetCurrentContext(),Nan::New("y").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0));

	v8::Local<v8::Object> objDPad = Nan::New<v8::Object>();
	objDPad->Set(Nan::GetCurrentContext(),Nan::New("left").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0));
	objDPad->Set(Nan::GetCurrentContext(),Nan::New("right").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0));
	objDPad->Set(Nan::GetCurrentContext(),Nan::New("up").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0));
	objDPad->Set(Nan::GetCurrentContext(),Nan::New("down").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0));

	v8::Local<v8::Object> objShoulder = Nan::New<v8::Object>();
	objShoulder->Set(Nan::GetCurrentContext(),Nan::New("left").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0));
	objShoulder->Set(Nan::GetCurrentContext(),Nan::New("right").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0));

	v8::Local<v8::Object> objThumb = Nan::New<v8::Object>();
	objThumb->Set(Nan::GetCurrentContext(),Nan::New("left").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0));
	objThumb->Set(Nan::GetCurrentContext(),Nan::New("right").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0));

	v8::Local<v8::Object> objControl = Nan::New<v8::Object>();
	objControl->Set(Nan::GetCurrentContext(),Nan::New("back").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0));
	objControl->Set(Nan::GetCurrentContext(),Nan::New("start").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0));


	v8::Local<v8::Object> objTrigger = Nan::New<v8::Object>();
	objTrigger->Set(Nan::GetCurrentContext(),Nan::New("left").ToLocalChecked(), Nan::New((float)controllerState.Gamepad.bLeftTrigger / 255));
	objTrigger->Set(Nan::GetCurrentContext(),Nan::New("right").ToLocalChecked(), Nan::New((float)controllerState.Gamepad.bRightTrigger / 255));


	v8::Local<v8::Object> objLeftStick = Nan::New<v8::Object>();
	objLeftStick->Set(Nan::GetCurrentContext(),Nan::New("x").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbLX));
	objLeftStick->Set(Nan::GetCurrentContext(),Nan::New("y").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbLY));

	v8::Local<v8::Object> objRightStick = Nan::New<v8::Object>();
	objRightStick->Set(Nan::GetCurrentContext(),Nan::New("x").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbRX));
	objRightStick->Set(Nan::GetCurrentContext(),Nan::New("y").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbRY));


	v8::Local<v8::Object> obj = Nan::New<v8::Object>();
	// Digital, i.e. buttons
	obj->Set(Nan::GetCurrentContext(),Nan::New("buttons").ToLocalChecked(), objButtons);
	obj->Set(Nan::GetCurrentContext(),Nan::New("dpad").ToLocalChecked(), objDPad);
	obj->Set(Nan::GetCurrentContext(),Nan::New("shoulder").ToLocalChecked(), objShoulder);
	obj->Set(Nan::GetCurrentContext(),Nan::New("thumb").ToLocalChecked(), objThumb);
	obj->Set(Nan::GetCurrentContext(),Nan::New("control").ToLocalChecked(), objControl);

	// Analog, i.e. sticks and triggers
	obj->Set(Nan::GetCurrentContext(),Nan::New("trigger").ToLocalChecked(), objTrigger);
	obj->Set(Nan::GetCurrentContext(),Nan::New("leftstick").ToLocalChecked(), objLeftStick);
	obj->Set(Nan::GetCurrentContext(),Nan::New("rightstick").ToLocalChecked(), objRightStick);



	info.GetReturnValue().Set(obj);
}
