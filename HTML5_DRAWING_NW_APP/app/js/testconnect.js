var net = require('net');

var MAXIUM_MSG_LENGTH = 64;
var SOFTWARE_VERSION = 1234;
var MINUM_MSG_LENGTH = 4;

var TCP_CONNECTION_PORT = 20121;
var TCP_DATA_PORT = 20123;
var TCP_REMOTE_DISPLAY_PORT = 20125;

var cmd_socketId;
var data_socketId;
var screenshot_socketId;

var clientName;
var rvaIP = "";
var isPIN = false;
var pinCode;
var client_id = 0;
var user_id = 0;
var state = 0;
var osDevType;
var screen_number = 0;
var image_value;
var ssid;
var isDataConnected = false;
var isScreenConnected = false;
var isCMDConnected = false;
var network_mode;
var dev_name;
var isPause = false;

var screenshot_userid = 0;
var isGroupMode;
var mainWindow;

var display_width = 0;
var display_height = 0;

var nvc_model;
var rva_version;
var isVersionMatch = false;

var isSplitMode = false;
var lastImageTime;
var nextImageTime;

var isImageSendingPause = false;

var client_name = "test";
var HOST = "192.168.1.17";
var pin = 0;

function send_CMD_CONNECTION_REQUEST() {

    var buf = new Uint8Array(MAXIUM_MSG_LENGTH);

    for (var i = 0; i < MAXIUM_MSG_LENGTH; i++) {
        buf[i] = 0;
    }

    buf[1] = 124; // message type
    buf[2] = 0; // client ID
    buf[3] = 0; // state

    var dataPortArray = convert_int_into_bytes_big_endian(pin);
    buf[4] = dataPortArray[0];
    buf[5] = dataPortArray[1];
    buf[6] = dataPortArray[2];
    buf[7] = dataPortArray[3];

    var pversion = convert_int_into_bytes_big_endian(1234);
    buf[8] = pversion[0];
    buf[9] = pversion[1];
    buf[10] = pversion[2];
    buf[11] = pversion[3];

    buf[12] = 6; //OS_DEV_TYPE.OS_CHROME_PC; // ChromeBook = 9
    if (isPIN) {
        buf[13] = 1;
    } else {
        buf[13] = 0;
    }

    var width = convert_int_into_bytes_big_endian(0);
    buf[14] = width[0];
    buf[15] = width[1];
    buf[16] = width[2];
    buf[17] = width[3];

    var height = convert_int_into_bytes_big_endian(0);
    buf[18] = height[0];
    buf[19] = height[1];
    buf[20] = height[2];
    buf[21] = height[3];


    length = client_name.length;

    if (length > 0) {
        var data = convert_str_into_bytes(client_name);
        length = data.length;

        for (var k = 0; k < length; k++) {
            buf[22 + k] = data[k];
        }
    }
    buf[0] = length + MINUM_MSG_LENGTH + 18;
    console.log('Data to send: ');
    console.log(buf);
    
    // creating node buffer
    var node_buf = new Buffer(buf);

    console.log('TRYING TO CONNECT TO: ' + HOST + ':' + TCP_CONNECTION_PORT);
    var client = new net.Socket();
    client.connect(TCP_CONNECTION_PORT, HOST, function () {

        console.log('CONNECTED TO: ' + HOST + ':' + TCP_CONNECTION_PORT);
        // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client    
        client.write(node_buf);
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
    /*
	
	chrome.sockets.tcp.send( cmd_socketId, buf.buffer.slice(0,length+MINUM_MSG_LENGTH+18), function(sendInfo) {
		if (sendInfo.resultCode == 0) {
			console.log("Connection Request Success");
			printTime();
		} else {
			console.log("Connection Request Fail");
			printTime();
		}
	});*/
}