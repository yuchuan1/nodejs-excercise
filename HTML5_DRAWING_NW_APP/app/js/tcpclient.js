var net = require('net');

var HOST = '127.0.0.1';
var PORT = 9050;

var client = new net.Socket();
client.connect(PORT, HOST, function () {

    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client 
   // client.write('I am Eddie!');
    
    var length = 0;
	
	var buf = new Uint8Array(64);
	
	for (var i = 0; i < 64; i++) {
		buf[i] = 0;
	}
	
	buf[1] = 124; // message type
 	buf[2] = 0; // client ID
	buf[3] = 0; // state
	
	var dataPortArray = convert_int_into_bytes_big_endian(0);
	buf[4] = dataPortArray[0];
	buf[5] = dataPortArray[1];
	buf[6] = dataPortArray[2];
	buf[7] = dataPortArray[3];
	
	var pversion = convert_int_into_bytes_big_endian(1234);
	buf[8] = pversion[0];
	buf[9] = pversion[1];
	buf[10] = pversion[2];
	buf[11] = pversion[3];
	
	buf[12] = 6;//OS_DEV_TYPE.OS_CHROME_PC; // ChromeBook = 9
	if (isPIN) {
		buf[13] = 1;
	} else {
		buf[13] = 0;
	}
	
	var width = convert_int_into_bytes_big_endian(display_width);
	buf[14] = width[0];
	buf[15] = width[1];
	buf[16] = width[2];
	buf[17] = width[3];
	
	var height = convert_int_into_bytes_big_endian(display_height);
	buf[18] = height[0];
	buf[19] = height[1];
	buf[20] = height[2];
	buf[21] = height[3];
    
    client.write(buf);

});

// Add a 'data' event handler for the client socket
// data is what the server sent to this socket
client.on('data', function (data) {

    console.log('DATA: ' + data);
    // Close the client socket completely
    client.destroy();

});

// Add a 'close' event handler for the client socket
client.on('close', function () {
    console.log('Connection closed');
});