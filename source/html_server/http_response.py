class HTTPResponse ():
    def __init__(self, content=b'', content_type='text/html', reason='OK', status=200,  headers=None):

        self.content_type = content_type
        self.status = status
        self.headers = headers
        self.content = content
        self.reason = reason

    def message (self):

        msg = 'HTTP/1.1 {s} {r} \r\n'.format(s=self.status, r=self.reason).encode()
        msg += 'Content-Type: {c} \r\n'.format(c=self.content_type).encode()
        msg += 'Content-Lenght: {l} \r\n'.format(l=len(self.content)).encode()
        msg += b'\r\n'
        msg += self.content

        return msg