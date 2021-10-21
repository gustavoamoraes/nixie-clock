class RGBLED ():
    def __init__(self, color_callback, ocilator):
        self.color_callback = color_callback
        self.ocilator = ocilator
        self.base_color = (255,255,255);

    @property
    def color():
        return color_callback()

    def oscillating_color ():
        base_color = self.base_color
        q = self.ocilator.positive_ocilation(1, self.current_duty)
        return (base_color[0]*q, base_color[1]*q, base_color[2]*q)

    def negative_color ():
        base_color = self.base_color
        return (255-base_color[0], 255-base_color[1], 255-base_color[2])

    def constant_color ():
        return self.base_color