var fs = require('fs');
var protobuf = require('protocol-buffers')
var messages = protobuf(fs.readFileSync('test.proto'))
var buf = messages.Test.encode({
    num: 42,
    payload: 'hello world'
});
console.log(buf);
var obj = messages.Test.decode(buf);
console.log(obj);
