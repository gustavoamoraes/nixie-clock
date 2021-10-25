class Event ():

    def __init__ (self):
        self.handlers = []

    def __add__ (self, a):
        self.handlers.append(a)
        return self

    def __call__ (self, *args, **kwargs):
        for handler in self.handlers:
            handler(*args, **kwargs)