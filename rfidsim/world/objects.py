import numpy as np


class Object3d(object):
    def __init__(self):
        self.owner = None
        self._position = np.array([0.0, 0.0, 0.0])

    @property
    def pos(self):
        raise NotImplementedError()
