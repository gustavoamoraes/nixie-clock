from ring_module import RingModule

class TestModule (RingModule):

    def __init__(self):
        super().__init__("Test")

    def on_changed (self, q):
        print('test: ' + str(q))
    def on_select (self):
        print('test selected')

    def on_back(self):
        print('back')
        super().on_back()