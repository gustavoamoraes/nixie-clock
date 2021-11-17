from profile import Profile

class RingModule:

    def __init__(self, name):

        self.childs = [] # Other ring modules
        self.profile = Profile.default()
        self.name = name 
        self.active = False   

    #Returns first active child 
    @property
    def active_child (self):
        try:
            return next(c for c in self.childs if c.active)
        except StopIteration:
            return None

    def on_active_child (child_func):
        def wrapper(parent_func):
            def _wrapper (self, *args, **kwargs):
                if self.active_child:
                    return child_func (self.active_child)(*args, **kwargs)
                return parent_func (self, *args, **kwargs)
            return _wrapper
        return wrapper

    #Gets called when a step is detected
    def on_changed (self, q):
        return None
    #Gets called when the select button is pressed
    def on_select (self):
        return None
    #Gets called when the back button is pressed
    def on_back (self):
        self.active = False
    #Gets called every frame
    def update (self):
        return None