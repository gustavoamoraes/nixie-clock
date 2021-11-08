from ring_module import RingModule
from profile import Profile

class MainModule(RingModule):
    def __init__(self, childs):
        super().__init__("Main Module")

        self.childs = childs
        self.active = True
        self.ring_index = 0

        self.profile.ring_colors[0] = (255,0,0)
        self.profile.ring_colors[1] = (0,255,0)
        self.profile.ring_colors[2] = (0,0,255)

    @RingModule.on_active_child( lambda active_child: active_child.on_changed )
    def on_changed (q):
        self.profile.color_callbacks[self.ring_index] = RGBLED.constant_color
        self.ring_index = (self.ring_index + q) % len(self.childs)
        self.profile.color_callbacks[self.ring_index] = RGBLED.oscillating_color

    @RingModule.on_active_child( lambda active_child: active_child.on_select )
    def on_select ():
        self.childs[self.ring_index].active = True

    @RingModule.on_active_child( lambda active_child: active_child.on_back )
    def on_back ():
        pass

    @RingModule.on_active_child( lambda active_child: active_child.update )
    def update():
        index_child = self.childs[self.ring_index]
        index_child.update()
        self.profile.display_number = index_child.profile.display_number
        return self.profile