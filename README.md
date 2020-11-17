# QTcpSocket

A simple Qt client-server TCP architecture to transfer data between peers.

![Application](https://github.com/manfredipist/QTcpSocket/blob/master/images/demo.png?raw=true)

The Architecture is made up of 2 projects:
- QTCPServer
- QTCPClient

You can instantiate as many QTCPClient as you wish.

## Features

1. Broadcast transferring
2. Single channel transferring
3. Media transferring capabilities (*.json, *.txt, *.png, *.jpg, *.jpeg)  
4. Updated to latest slot-signal syntax found in Qt 5.15
5. Non-redundant design

## Protocol

The protocol i've designed to enable data transferring is pretty straightforward, each message sent is made up of:
- A 128 byte fixed-size header
- A Non-fixed size data buffer

![Protocol schema](https://github.com/manfredipist/QTcpSocket/blob/master/images/protocol.png?raw=true)

The header follows the following structure:
```
QString header = QString("fileType:%1, fileName:%2, fileSize:%3;")
```
Where:
- ```%1``` can be "attachment" or "message";
- ```%2``` is "null" for messages, the correspondent fileName for attachments sent;
- ```%3``` is the size of the message or the attachment sent.

Many improvements could have been made during protocol design, one among many, to specify header size in the header and save up some byte during transfers, especially messages ones.

__*This protocol is meant to be used only for recreative purposes as it isn't compliant at all to the FTP quality standards, that we are used to, in these days.*__
