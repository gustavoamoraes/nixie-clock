from main import RingModule
import constants 

class RingModule:

    def __init__(self, name):

        self.childs = [] # Other ring modules
        self.profile = Profile.default()
        self.name = name 
        self.active = False   

    #Returns first active child 
    @property
    def active_child ():
        return next((c for c in self.childs if c.active), None)

    def on_active_child (child_func):
        def wrapper(parent_func):
            def _wrapper (self, *args, **kwargs):
                if self.active_child:
                    return child_func (self.active_child)(*args, **kwargs)
                return parent_func (self, *args, **kwargs)
            return _wrapper
        return wrapper

    #Gets called when a step is 
    def on_changed (self, q):
        return None
    #Gets called when the select button is pressed
    def on_select (self):
        return None
    #Gets called when the back button is pressed
    def on_back (self):
        return None
    #Updates module
    def update (self):
        return None