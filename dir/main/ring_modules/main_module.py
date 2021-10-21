from ring_module import RingModule
from constants import DIGIT_COUNT
from profile import Profile

class MainModule(RingModule):
    def __init__(self, childs):
        super().__init__("Main Module")

        self.childs = childs
        self.active = True
        self.ring_index = 0

    @RingModule.on_active_child( lambda active_child: active_child.on_changed )
    def on_changed (q):
        self.ring_index = (self.ring_index + q) % len(self.childs)

    @RingModule.on_active_child( lambda active_child: active_child.on_select )
    def on_select ():
        self.childs[self.ring_index].active = True

    @RingModule.on_active_child( lambda active_child: active_child.on_back )
    def on_back ():
        super().on_back()

    @RingModule.on_active_child( lambda active_child: active_child.update )
    def update():
        index_child = self.childs[self.ring_index]
        index_child.update()
        self.profile.display_number = index_child.profile.display_number

        return self.profile