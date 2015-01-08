var async = require('async');
var ip = require('ip');

function step1(callback){
    setTimeout(function(){console.log(1);callback();
                         }, 500);
}

function step2(callback){
    console.log(2);
    callback();
}

function step3(callback){
   console.log(3);
    callback();
}


var array_of_functions = [step1, step2];
async.series(array_of_functions);

console.log(ip.toLong('192.168.43.243'));
