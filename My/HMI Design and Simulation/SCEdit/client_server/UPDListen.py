# -*- coding: utf-8 -*-
import socket
from threading import Thread
from socketserver import ThreadingMixIn

# ipv4        SOCK_DGRAM指定了这个Socket的类型是UDP
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
# 绑定 客户端口和地址:
s.bind(('', 9999))
print('Bind UDP on 9999...')
while True:
    # 接收数据 自动阻塞 等待客户端请求:
    data, addr = s.recvfrom(1024)
    print('Received from %s:%s.' % (addr,data))
    s.sendto(str.encode('Hello, %s!' % data), addr)
#recvfrom()方法返回数据和客户端的地址与端口，这样，服务器收到数据后，直接调用sendto()就可以把数据用UDP发给客户端。