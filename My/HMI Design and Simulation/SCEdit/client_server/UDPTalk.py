# -*- coding: utf-8 -*-
 
import socket
import time
from threading import Thread
import operator

MyName="Jone"			
class ClientThread(Thread):

    def __init__(self,ip,port,sock):
        Thread.__init__(self)
        self.ip = ip
        self.port = port
        self.sock = sock
        print(" New thread started for "+ip+":"+str(port))

    def run(self):
	    ss = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	    ss.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
	    ss.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	    ss.bind(('', 9999))
	    print('Bind UDP on 9999...')
	    while True:
		    data, addr = ss.recvfrom(1024)
		    print('Received from %s:%s.' % (addr,data))
		    if operator.eq(data,str.encode("who is there?")):
			    self.sock.sendto(str.encode('this is %s!' % MyName), addr)
				

			

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
newthread = ClientThread('<broadcast>',9999,s)
newthread.start()

# 发送数据:
s.sendto(str.encode("who is there?"), ('<broadcast>', 9999))

while True:
	data, addr = s.recvfrom(1024)
	print('Received from %s:%s.' % (addr,data))


newthread.join()