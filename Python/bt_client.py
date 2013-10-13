# file: rfcomm-client.py
# auth: Albert Huang <albert@csail.mit.edu>
# desc: simple demonstration of a client application that uses RFCOMM sockets
#       intended for use with rfcomm-server
#
# $Id: rfcomm-client.py 424 2006-08-24 03:35:54Z albert $

from bluetooth import *
import sys
import socket
UDP_IP = "127.0.0.1"
UDP_PORT = 5005

addr = None

# if len(sys.argv) < 2:
#     print "no device specified.  Searching all nearby bluetooth devices for"
#     print "the SampleServer service"
# else:
addr = '98:D6:F7:71:8C:DB'  # sys.argv[1]
print "Searching for SampleServer on %s" % addr

# search for the SampleServer service
uuid = "446118f0-8b1e-11e2-9e96-0800200c9a66"
service_matches = find_service(uuid=uuid, address=addr)

if len(service_matches) == 0:
    print "couldn't find the SampleServer service =("
    sys.exit(0)

first_match = service_matches[0]
port = first_match["port"]
name = first_match["name"]
host = first_match["host"]

print "connecting to \"%s\" on %s" % (name, host)

# Create the client socket
sock = BluetoothSocket(RFCOMM)
sock.connect((host, port))
udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
udp_sock.bind((UDP_IP, UDP_PORT))

print "connected.  type stuff"
while True:
    data, addr = udp_sock.recvfrom(1024)  # buffer size is 1024 bytes
    print "received message:", data
    #data = raw_input()
    if len(data) == 0:
        break
    sock.send(data)

sock.close()
