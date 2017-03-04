from rfidsim.radio import antennas
from rfidsim.world import scene


class Medium(object):
    def __init__(self):
        self.scene = None
        self.preferred_path_loss_model = None

    def get_path_loss(self, tx_antenna, rx_antenna):
        pass

