class RGBLED ():
    def __init__(self, color_callback, ocilator):
        self.color_callback = color_callback
        self.ocilator = ocilator
        self.base_color = (255,255,255);

    def color(self):
        return self.color_callback(self)

    def oscillating_color (self):
        base_color = self.base_color
        q = self.ocilator.positive_ocilation(1, self.current_duty)
        return (base_color[0]*q, base_color[1]*q, base_color[2]*q)

    def negative_color (self):
        base_color = self.base_color
        return (255-base_color[0], 255-base_color[1], 255-base_color[2])

    def constant_color (self):
        return self.base_color