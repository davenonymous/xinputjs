#include "XInputFunctions.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
	Nan::Set(target, Nan::New("IsConnected").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(IsConnected)).ToLocalChecked());
	Nan::Set(target, Nan::New("GetState").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(GetState)).ToLocalChecked());
	Nan::Set(target, Nan::New("Vibrate").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(Vibrate)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, InitAll);