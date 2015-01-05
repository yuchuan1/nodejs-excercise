var binary = require('node-pre-gyp');
var path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')));
var binding = require(binding_path);

//require('assert').equal(binding.hello("hello from c++"),"hello from c++");
//require('assert').equal(binding.hello(),"hello");

//console.log(binding.hello('Hello from JavaScript and C'));
//console.log(binding.hello());
//console.log(binding.hello("test from JS"));
//require('assert').equal(binding.hello("hello from c++"),"hello from c++");
//console.log(binding.hello("hello from c++"));
//console.log(binding.setResolution(800, 600));
console.log(binding.setResolution(1366, 768));
//console.log(binding.hello(" Hello JavaScript"));
//process.exit(0);
