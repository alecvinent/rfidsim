from rfidsim import world
from rfidsim.c1g2.phy import antennas


class Channel(object):
    def __init__(self):
        self.path_loss_model = None
        self.scene = None

    def __str__(self):
        return "Channel"

    def get_path_loss(self, tx_antenna, rx_antenna):
        pass
