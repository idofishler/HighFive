#!/usr/bin/env python

import serial
from datetime import datetime
#import msvcrt

ser = serial.Serial("COM4", 38400)
while True:
    s = str(ser.readline())
    #c = msvcrt.getch()
    n = int(s.split(" / ")[0])
#    print n
    if n > 0:
        t = datetime.now()
        #print str(datetime.now())
        
        print (str(t.hour) + ',' + str(t.minute) +',' + str(t.second)+',' +str(t.microsecond))
        
#    now=raw_input('please press enter to prit time')
#    print"keypress"+str(datetime.now())
    #c = msvcrt.getch()
    #print 'you pressed a key ' +str(datetime.now())