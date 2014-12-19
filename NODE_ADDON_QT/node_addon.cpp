// v8
#include <v8.h>

// node.js
#include <node.h>
#include <node_version.h>
#include <mylib/interface.h>
#include <iostream>

#if (NODE_MODULE_VERSION > 0x000B)

    static void get_hello(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::HandleScope scope(v8::Isolate::GetCurrent());
        
        v8::String::Utf8Value  param1(args[0]->ToString());
        std::string mystr = std::string(*param1);   
        MyLib::Message msg(mystr);

        std::string msg_string = msg.get();
        args.GetReturnValue().Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(),msg_string.c_str()));
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

#endif

extern "C" {
    static void start(v8::Handle<v8::Object> target) {
#if (NODE_MODULE_VERSION > 0x000B)
        v8::HandleScope scope(v8::Isolate::GetCurrent());
#else
        v8::HandleScope scope;
#endif
        NODE_SET_METHOD(target, "hello", get_hello);
    }
}

NODE_MODULE(node_addon, start)