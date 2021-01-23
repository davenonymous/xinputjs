#include "XInputFunctions.h"
#include <XInput.h>

NAN_METHOD(Vibrate) {
	//DEPRECATION FIX
	v8::Isolate* isolate = info.GetIsolate();
    	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	double left = info[1]->IsUndefined() ? 0 : Nan::To<double>(info[1]).FromJust();
	double right = info[2]->IsUndefined() ? 0 : Nan::To<double>(info[2]).FromJust();

	vibration.wLeftMotorSpeed = (WORD)(65535 * left);
	vibration.wRightMotorSpeed = (WORD)(65535 * right);

	// TODO: Verify parameters
	XInputSetState((WORD)info[0]->IntegerValue(context).FromJust(), &vibration);
}

NAN_METHOD(IsConnected) {
	//DEPRECATION FIX
	v8::Isolate* isolate = info.GetIsolate();
    	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	
	XINPUT_STATE controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	// TODO: Verify parameters
	DWORD result = XInputGetState((WORD)info[0]->IntegerValue(context).FromJust(), &controllerState);
	if(result == ERROR_SUCCESS) {
		info.GetReturnValue().Set(true);
	} else {
		info.GetReturnValue().Set(false);
	}
}

NAN_METHOD(GetState) {
	//DEPRECATION FIX
	v8::Isolate* isolate = info.GetIsolate();
    	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	
	XINPUT_STATE controllerState;
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	// TODO: Verify parameters
	DWORD result = XInputGetState((WORD)info[0]->IntegerValue(context).FromJust(), &controllerState);


	v8::Local<v8::Object> objButtons = Nan::New<v8::Object>();
	objButtons->Set(context, Nan::New("a").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0));
	objButtons->Set(context, Nan::New("b").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0));
	objButtons->Set(context, Nan::New("x").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0));
	objButtons->Set(context, Nan::New("y").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0));

	v8::Local<v8::Object> objDPad = Nan::New<v8::Object>();
	objDPad->Set(context, Nan::New("left").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0));
	objDPad->Set(context, Nan::New("right").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0));
	objDPad->Set(context, Nan::New("up").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0));
	objDPad->Set(context, Nan::New("down").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0));

	v8::Local<v8::Object> objShoulder = Nan::New<v8::Object>();
	objShoulder->Set(context, Nan::New("left").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0));
	objShoulder->Set(context, Nan::New("right").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0));

	v8::Local<v8::Object> objThumb = Nan::New<v8::Object>();
	objThumb->Set(context, Nan::New("left").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) != 0));
	objThumb->Set(context, Nan::New("right").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) != 0));

	v8::Local<v8::Object> objControl = Nan::New<v8::Object>();
	objControl->Set(context, Nan::New("back").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0));
	objControl->Set(context, Nan::New("start").ToLocalChecked(), Nan::New((controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0));


	v8::Local<v8::Object> objTrigger = Nan::New<v8::Object>();
	objTrigger->Set(context, Nan::New("left").ToLocalChecked(), Nan::New((float)controllerState.Gamepad.bLeftTrigger / 255));
	objTrigger->Set(context, Nan::New("right").ToLocalChecked(), Nan::New((float)controllerState.Gamepad.bRightTrigger / 255));


	v8::Local<v8::Object> objLeftStick = Nan::New<v8::Object>();
	objLeftStick->Set(context, Nan::New("x").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbLX));
	objLeftStick->Set(context, Nan::New("y").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbLY));

	v8::Local<v8::Object> objRightStick = Nan::New<v8::Object>();
	objRightStick->Set(context, Nan::New("x").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbRX));
	objRightStick->Set(context, Nan::New("y").ToLocalChecked(), Nan::New(controllerState.Gamepad.sThumbRY));


	v8::Local<v8::Object> obj = Nan::New<v8::Object>();
	// Digital, i.e. buttons
	obj->Set(context, Nan::New("buttons").ToLocalChecked(), objButtons);
	obj->Set(context, Nan::New("dpad").ToLocalChecked(), objDPad);
	obj->Set(context, Nan::New("shoulder").ToLocalChecked(), objShoulder);
	obj->Set(context, Nan::New("thumb").ToLocalChecked(), objThumb);
	obj->Set(context, Nan::New("control").ToLocalChecked(), objControl);

	// Analog, i.e. sticks and triggers
	obj->Set(context, Nan::New("trigger").ToLocalChecked(), objTrigger);
	obj->Set(context, Nan::New("leftstick").ToLocalChecked(), objLeftStick);
	obj->Set(context, Nan::New("rightstick").ToLocalChecked(), objRightStick);

	

	info.GetReturnValue().Set(obj);
}
