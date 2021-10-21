class RotaryEncoder ():
    def __init__(self, clk, dt):

        self.clk = clk
        self.last_clk = clk
        self.dt = dt

    @property
    def direction (self):
        dir = (-1 if self.clk.value() == self.dt.value() else 1) if not self.clk.value() == self.last_clk else 0
        self.last_clk = self.clk.value()
        return dir