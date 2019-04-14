# -*- coding: utf-8 -*-
 
import socket
import time
from threading import Thread

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
	    ss.bind(('',9998))
	    print('Bind UDP on 9999...')
	    while True:
		    data, addr = ss.recvfrom(1024)
		    print('Received from %s:%s.' % (addr,data))

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
newthread = ClientThread('<broadcast>',9999,s)
newthread.start()

# 发送数据:
s.sendto(str.encode("who is there?"), ('<broadcast>', 9999))
# 接收数据:
print(s.recv(1024))


while True:
	time.sleep(5)
	


newthread.join()