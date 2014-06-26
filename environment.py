from numpy import *
import random as rd

random.seed()

EMPTY = 0
PREY1 = 1
PREY2 = 2
PREDATOR = 3

class Environment:
    """Simulate an environment with predators and two types of prey
    
    The environment is contained as a two dimensional matrix of Squares.
    The two dimensions correlate to spatial dimensions
    """
    def __init__(self, size, probability_map):
        self._data = empty((size, size), dtype=object)
        self._next = empty((size, size), dtype=object)
        self.int_repr = empty((size, size), dtype=int)
        self.probability_map = probability_map
        self.size = size
        self._init_values()

    def _init_values(self):
        for i in range(0, self.size):
            for j in range(0, self.size):
                self._data[i,j] = Square(self.probability_map)
                self._next[i,j] = Square(EMPTY)

    def __getitem__(self, key):
        if len(key) == 2:
            return self._data[key[0] % self.size, key[1] % self.size]
        else:
            raise ValueError('must pass a tuple of len 2 ie. x[4,5]')

    def __setitem__(self, key, value):
        if len(key) == 2:
            self._next[key[0], key[1]] = value
        else:
            raise ValueError('must pass a tuple of len 2 , ie. x[4,5]')

    def refresh(self):
        """call after you have updated a "frame"."""
        self._data, self._next = self._next, self._data
        for i in range(0, self.size):
            for j in range(0, self.size):
                self.int_repr[i,j] = int(self[i,j])

    def neighbors(self, x, y):
        neighborList = []
        for i in (-1, 0, 1):
            for j in (-1, 0, 1):
                if i != 0 or j != 0:
                    neighborList.append(self[x + i, y + j])
        return neighborList

class Square:
    __slots__ = ('kind', 'repRate', 'eats')
    def __init__(self, arg1, repRate = None, eats = None):
        if type(arg1) == int:
            #arg1 is the kind
            self.kind = arg1
            self.repRate = repRate
            self.eats = eats
        else:
            #arg1 is probability_map
            rand = rd.random()
            if rand <= arg1["Empty"]:
                self.kind = EMPTY
                self.repRate, self.eats = None, None
            elif rand <= arg1["Empty"] + arg1["Prey1"]:
                self.kind = PREY1
                self.repRate, self.eats = None, None
            elif rand <= arg1["Empty"] + arg1["Prey1"] + arg1["Prey2"]:
                self.kind = PREY2
                self.repRate, self.eats = None, None
            else:
                self.kind = PREDATOR
                self.repRate = rd.random()
                self.eats = rd.randrange(1,3)

    def __eq__(self, other):
        if type(other) == int:
            return self.kind == other
        elif type(other) == type(self):
            return self.kind == other.kind

    def __ne__(self, other):
        return not self == other
    
    def __repr__(self):
        return "<Square with kind={0}, repRate={1}, eats={2}>".format(self.kind, self.repRate, self.eats)

    def __int__(self):
        return self.kind
