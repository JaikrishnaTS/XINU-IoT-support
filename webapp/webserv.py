#!/usr/bin/env python

import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web
import time
import sys, socket
import threading
import signal
import datetime
from devconf import devconf, local_ip, local_port, minSun, maxSun

conWS = None # currently supporting only one connection
locserver_address = (local_ip, local_port)
insock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
outsock = None
ledvals = []

class WSHandler(tornado.websocket.WebSocketHandler):
    def open(self):
        global conWS
        conWS = self
        print 'New Connection' 

    def sendMarquee(self, MarqData):
        print 'sent data:', MarqData
        try:
            self.write_message(MarqData)
        except:
            print "couldn't write message", MarqData

    def on_message(self, message):
        configUpdate(message)

    def on_close(self):
        global conWS
        conWS = None
        print 'Connection Closed'

def stopall():
    print "stopping all"
    stopTornado()

def updateTime(): # not being used
    global conWS
    t = time.asctime()
    if conWS:
        conWS.sendMarquee(t)

def runTornado(): # thread
    tornado.ioloop.IOLoop.instance().start()

def stopTornado():
    ioloop = tornado.ioloop.IOLoop.instance()
    ioloop.add_callback(ioloop.stop)
    print "asked to stop"

def recvLDRval(): # thread
    global conWS, insock, ledvals
    insock.bind(locserver_address)
    while True:
        data, address = insock.recvfrom(4096)
        if data and conWS:
            val = (int(data) - minSun)*100/(maxSun - minSun)
            if val > 100:
                val = 100
            if val < 0:
                val = 0
            conWS.sendMarquee(str(val))
        if data:
            ledvals.append(int(data))

def refresher(): # thread
    global outsock, devconf, ledvals
    outsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    while True:
        now = datetime.datetime.now()
        currtime = now.hour*60 + now.minute
        for room, roomconf in devconf.iteritems():
            if 'update' in roomconf:
                #sent = outsock.sendto(' '.join(map(str, [1, roomconf['color' ]])), (roomconf['ip'], roomconf['port']))
                sent = outsock.sendto(' '.join(map(str, [0, roomconf['energy']])), (roomconf['ip'], roomconf['port']))
                if roomconf['mode'] == 1:
                    if roomconf['startTime'] < roomconf['stopTime']:
                        if currtime >= roomconf['startTime'] and currtime < roomconf['stopTime']:
                            sent = outsock.sendto(' '.join(map(str, [1, roomconf['color']])), (roomconf['ip'], roomconf['port']))
                            roomconf['state'] = True
                        else:
                            sent = outsock.sendto(' '.join(map(str, [1, 0])), (roomconf['ip'], roomconf['port']))
                            roomconf['state'] = False
                    else:
                        if currtime >= roomconf['startTime'] or currtime < roomconf['stopTime']:
                            sent = outsock.sendto(' '.join(map(str, [1, roomconf['color']])), (roomconf['ip'], roomconf['port']))
                            roomconf['state'] = True
                        else:
                            sent = outsock.sendto(' '.join(map(str, [1, 0])), (roomconf['ip'], roomconf['port']))
                            roomconf['state'] = False
                elif roomconf['mode'] == 2:
                    if len(ledvals) > 0:
                        currled = ledvals[-1]
                        newint = None
                        if minSun < currled < maxSun:
                            currled -= minSun
                            newint = (currled*100)/(maxSun-minSun)
                            newint = 100 - newint
                        elif currled >= maxSun:
                            newint = 0
                        elif currled <= minSun:
                            newint = 100
                        print "newint" , newint
                        spcol = [(roomconf['color'] & 0xFF0000) >> 16, (roomconf['color'] & 0xFF00) >> 8, roomconf['color'] & 0xFF]
                        newper = roomconf['minInt'] + (roomconf['maxInt'] - roomconf['minInt'])*newint/100
                        spcol = map(int, [c*float(newper)/100 for c in spcol])
                        print "spcol", spcol
                        newcol = (spcol[0] << 16) | (spcol[1] << 8) | spcol[2]
                        sent = outsock.sendto(' '.join(map(str, [1, newcol])), (roomconf['ip'], roomconf['port']))
                        roomconf['oldint'] = newint
                else:
                    sent = outsock.sendto(' '.join(map(str, [1, roomconf['color' ]])), (roomconf['ip'], roomconf['port']))
                del devconf[room]['update']
                continue
            if roomconf['mode'] == 1:
                if currtime == roomconf['startTime'] and roomconf['state'] == False:
                    sent = outsock.sendto(' '.join(map(str, [1, roomconf['color']])), (roomconf['ip'], roomconf['port']))
                    roomconf['state'] = True
                elif currtime == roomconf['stopTime'] and roomconf['state'] == True:
                    sent = outsock.sendto(' '.join(map(str, [1, 0])), (roomconf['ip'], roomconf['port']))
                    roomconf['state'] = False
            elif roomconf['mode'] == 2:
                if len(ledvals) > 0:
                    currled = ledvals[-1]
                    newint = None
                    if minSun < currled < maxSun:
                        currled -= minSun
                        newint = (currled*100)/(maxSun-minSun)
                        newint = 100 - newint
                    elif currled >= maxSun:
                        newint = 0
                    elif currled <= minSun:
                        newint = 100
                    print "newint" , newint
                    if abs(roomconf['oldint'] - newint) > 10:
                        spcol = [(roomconf['color'] & 0xFF0000) >> 16, (roomconf['color'] & 0xFF00) >> 8, roomconf['color'] & 0xFF]
                        newper = roomconf['minInt'] + (roomconf['maxInt'] - roomconf['minInt'])*newint/100
                        spcol = map(int, [c*float(newper)/100 for c in spcol])
                        print "spcol", spcol
                        newcol = (spcol[0] << 16) | (spcol[1] << 8) | spcol[2]
                        sent = outsock.sendto(' '.join(map(str, [1, newcol])), (roomconf['ip'], roomconf['port']))
                        roomconf['oldint'] = newint
        time.sleep(5)

def configUpdate(message):
    global devconf
    message = map(int, message.split()) # roomNo mode color energy optional
    print message
    seldev = devconf[message[0]]
    seldev['mode'] = message[1]
    seldev['color'] = message[2]
    seldev['energy'] = message[3]
    if seldev['mode'] == 1:
        seldev['startTime'] = message[4]
        seldev['stopTime']  = message[5]
    elif seldev['mode'] == 2:
        seldev['minInt'] = message[4]
        seldev['maxInt'] = message[5]
        seldev['oldint'] = 100
    seldev['update'] = True
    print seldev
    #sent = outsock.sendto(' '.join(map(str, [1, seldev['color' ]])), (seldev['ip'], seldev['port']))
    #sent = outsock.sendto(' '.join(map(str, [0, seldev['energy']])), (seldev['ip'], seldev['port']))
    #sent = outsock.sendto(message, remserver_address)

application = tornado.web.Application([
    (r'/ws', WSHandler),
    (r'/(.*)', tornado.web.StaticFileHandler, {"path": "./dashboard/","default_filename": "dashboard.html"}),# WSHandler),
    #(r'/(.*)', tornado.web.StaticFileHandler, {"path": ".","default_filename": "index.html"}),# WSHandler),
])
 
 
if __name__ == "__main__":
    print "Starting Server"
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(9997)#(8888)
    print "starting torthread"
    torthread = threading.Thread(target=runTornado)
    torthread.start()
    print "starting recvLDRval"
    socklistener = threading.Thread(target=recvLDRval)
    socklistener.daemon = True
    socklistener.start()
    print "starting refresher"
    refresher = threading.Thread(target=refresher)
    refresher.daemon = True
    refresher.start()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        stopall()
    torthread.join()
