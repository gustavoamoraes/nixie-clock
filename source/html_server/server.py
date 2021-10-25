page_text = open('page.html', 'r').read()
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]

s = socket.socket()
s.bind(addr)
s.listen(1)

print('listening on', addr)

def server():

    while True:

        cl, addr = s.accept()
        print('client connected from', addr)
        cl_file = cl.makefile('rwb', 0)

        length = 0

        while True:
            line = cl.readline()
            if not line or line == b'\r\n':
                break
            if 'Content-Length: ' in line:
                length = int(line[16:-2])

        cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')

        if length:
            postquery = cl.read(length)
            request = ujson.loads(postquery)
                
        cl.sendall(page_text)
        cl.close()