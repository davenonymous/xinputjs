#ifndef NATIVE_EXTENSION_XINPUT
#define NATIVE_EXTENSION_XINPUT

#include <nan.h>

NAN_METHOD(IsConnected);
NAN_METHOD(GetState);
NAN_METHOD(Vibrate);

#endif