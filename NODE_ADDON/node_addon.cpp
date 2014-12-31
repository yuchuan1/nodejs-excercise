// v8
#include <v8.h>

// node.js
#include <node.h>
#include <node_version.h>
#include <mylib/interface.h>
#include <iostream>

// windows
#include <windows.h>

struct DDList {
    DISPLAY_DEVICE    Device;
    struct DDList    *Next;
};

static void DDList_Build(struct DDList **ppDD);
static void DDList_Clean(struct DDList **ppDD);
static BOOL DDList_Pop(struct DDList **ppDD, DISPLAY_DEVICE    *device);
static BOOL getResolution(int argc, char **argv, int *width, int *height);
struct DDList    *ddList        = NULL;

DISPLAY_DEVICE    device        = {0};
DEVMODE            deviceMode    = {0};
    int                newWidth    = 0;
    int                newHeight    = 0;
    int                actualWidth    = 0;
    int                actualHeight= 0;

#if (NODE_MODULE_VERSION > 0x000B)
    static void get_hello(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        DDList_Build(&ddList);
        device.cb = sizeof(device);
        if(DDList_Pop(&ddList, &device)) {
            deviceMode.dmSize            = sizeof(deviceMode);
            deviceMode.dmDriverExtra    = 0;
            if(EnumDisplaySettingsEx(device.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0)) {
                actualWidth = deviceMode.dmPelsWidth;
                actualHeight = deviceMode.dmPelsHeight;
            }
        }

        v8::HandleScope scope(v8::Isolate::GetCurrent());

        v8::String::Utf8Value  param1(args[0]->ToString());
        std::string mystr = std::string(*param1);
      //  MyLib::Message msg(mystr + " actualWidth:" + std::to_string(static_cast<long long>(actualWidth)));

        MyLib::Message msg(mystr);
        std::string msg_string = msg.get();
        args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), msg_string.c_str()));
    }

#else

    static v8::Handle<v8::Value> get_hello(const v8::Arguments& args)
    {
        v8::HandleScope scope;

        v8::String::Utf8Value  param1(args[0]->ToString());
        std::string mystr = std::string(*param1);
        MyLib::Message msg(mystr);
        std::string msg_string = msg.get();
        return scope.Close(v8::String::New(msg_string.c_str()));
    }

    static v8::Handle<v8::Value> get_screen_resolution(const v8::Arguments& args){
        DDList_Build(&ddList);
        device.cb = sizeof(device);
        if(DDList_Pop(&ddList, &device)) {
            deviceMode.dmSize            = sizeof(deviceMode);
            deviceMode.dmDriverExtra    = 0;
            if(EnumDisplaySettingsEx(device.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0)) {
                actualWidth = deviceMode.dmPelsWidth;
                actualHeight = deviceMode.dmPelsHeight;
            }
        }

        v8::HandleScope scope;

        v8::String::Utf8Value  param1(args[0]->ToString());
        std::string mystr = std::string(*param1);
        MyLib::Message msg(mystr);
        std::string msg_string = " Screen resolution info - \nactualWidth:" + std::to_string(static_cast<long long>(actualWidth))
            + "\nactualHeight:" + std::to_string(static_cast<long long>(actualHeight));
        return scope.Close(v8::String::New(msg_string.c_str()));
    }

#endif


static void DDList_Build(struct DDList **ppDD) {
    DWORD            deviceNum    = 0;
    DISPLAY_DEVICE    device        = {0};
    struct DDList    *ddList        = NULL;
    struct DDList    *dd            = NULL;
    struct DDList    *ddLastNode    = NULL;

    if(ppDD == NULL) {
        return;
    }
    *ppDD = NULL;

    //    build display devices list
    device.cb = sizeof(device);
    do {
        if(!EnumDisplayDevices(NULL, deviceNum, &device, 0)) {
            break;
        }
        if(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
            dd = (struct DDList *)malloc(sizeof(struct DDList));
            if(!dd) {
                break;
            }
            dd->Device    = device;
            dd->Next    = NULL;

            if(ddList) {
                ddLastNode->Next = dd;
                ddLastNode = dd;
            }
            else {
                ddList = dd;
                ddLastNode = ddList;
            }
        }
        deviceNum++;
    } while(1);

    *ppDD = ddList;
}

static BOOL DDList_Pop(struct DDList **ppDD, DISPLAY_DEVICE    *device) {
    struct DDList *ddList = NULL;
    struct DDList *ddHead = NULL;
    size_t cbDD = sizeof(DISPLAY_DEVICE);

    if(ppDD == NULL) {
        return FALSE;
    }
    if(device == NULL) {
        return FALSE;
    }
    if(device->cb < cbDD) {
        return FALSE;
    }
    ddList = *ppDD;
    CopyMemory(device, &ddList->Device, cbDD);

    ddHead = ddList;
    ddList = ddList->Next;
    free(ddHead);

    *ppDD = ddList;
    return TRUE;
}

static void DDList_Clean(struct DDList **ppDD) {
    struct DDList    *ddList        = NULL;
    struct DDList    *dd            = NULL;

    if(ppDD == NULL) {
        return;
    }
    ddList = *ppDD;
    //    clear list
    while(ddList) {
        dd = ddList;
        ddList = ddList->Next;
        free(dd);
    }
}

extern "C" {
    static void start(v8::Handle<v8::Object> target) {
#if (NODE_MODULE_VERSION > 0x000B)
        v8::HandleScope scope(v8::Isolate::GetCurrent());
#else
        v8::HandleScope scope;
#endif
        NODE_SET_METHOD(target, "hello", get_hello);
        NODE_SET_METHOD(target, "getResolution", get_screen_resolution);
    }
}

NODE_MODULE(node_addon, start)
