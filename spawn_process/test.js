var spawn = require('child_process').spawn;
var ls    = spawn('hello.exe');

ls.stdout.on('data', function (data) {
  console.log('pid:' + ls.pid + '\nstdout: ' + data);
});

ls.stderr.on('data', function (data) {
  console.log('stderr: ' + data);
});

ls.on('close', function (code) {
  console.log('child process exited with code ' + code);
});

/*
var exec = require('child_process').exec;
exec('dir', function(err, stdout, stderr) {
  console.log(stdout);
});
*/
