import matplotlib
matplotlib.use('TkAgg')

##=====================================
## Section 1: Import Modules
##=====================================

import random as RD
import pylab as PL
import environment as EV
import sys
import copy
import toml

##=====================================
## Section 2: Define Model Parameters
##=====================================


EMPTY, PREY1, PREY2, PREDATOR = 0,1,2,3
c = {}
#size = 50
#predatorP, prey1P, prey2P = .15, .15, .15
#emptyP = 1-(predatorP+prey1P+prey2P)
#both = False
#predatorR, prey1R, prey2R = .2, .2, .2
#deathR = .7
#repRateM, eatsM = .1, .00


##=====================================
## Section 3: Define Three Functions
##=====================================

def init():
    global env, c
    with open("config.toml") as conffile:
        c = toml.loads(conffile.read())

    c["dist"]["Empty"] = 1 - (c["dist"]["Prey1"] + c["dist"]["Prey2"] + c["dist"]["Predator"])
    datasave = []
    env = EV.Environment(c["size"], c["dist"])

def draw():
    env.draw()

def looping():
    for x in xrange(c["size"]):
        for y in xrange(c["size"]):
            if env._data[x,y] != env_prev._next[x][y]:
                return False
    return True

def opeats(e):
    if e == PREY2:
        return PREY1
    if e == PREY1:
        return PREY2

def cap(x):
    if x > 1: return 1
    elif x < 0: return 0
    else: return x

def step():
    global env, datasave
    for x in xrange(c["size"]):
        for y in xrange(c["size"]):
            env[x,y] = env[x,y]
            nbors = env.neighbors(x,y)
            if env[x,y]==EMPTY:
                p1=False
                p2=False
                if RD.random()<(1-(1-c["repRate"]["Prey1"])**len(filter((lambda x: x==PREY1), nbors))):
                    p1=True
                    env[x,y] = EV.Square(PREY1)
                if RD.random()<(1-(1-c["repRate"]["Prey2"])**len(filter((lambda x: x==PREY2), nbors))):
                    p2=True
                    env[x,y] = EV.Square(PREY2)
                if p1==True and p2 == True:
                    env[x,y] = EV.Square(RD.choice((PREY1,PREY2)))
            elif env[x,y] == PREY1 or env[x,y] == PREY2:
                opts = []
                for nbor in filter((lambda x: x==PREDATOR), nbors):
                    if ((nbor.eats < 0.5) == (env[x,y].kind == 1)) and RD.random()<nbor.repRate: #if nbor eats this, and can reproduce
                        opts.append(EV.Square(PREDATOR, cap(nbor.repRate+RD.gauss(0,c["mut"]["repRate"])), cap(nbor.eats + RD.gauss(0, c["mut"]["eats"]))))
                if len(opts) > 0:
                    env[x,y] = RD.choice(opts)
            elif env[x,y] == PREDATOR:
                if RD.random()<c["deathRate"]:
                    env[x,y] = EV.Square(EMPTY)
            else:
                print "environment wasn't a resonable value"
            del nbors
    env.refresh()

##=====================================
## Section 4: [Optional] Create Setter/Getter Functions for Model Parameters
##=====================================


##=====================================
## Section 5: Import and Run GUI
##=====================================

import pycxsimulator
pycxsimulator.GUI(title='My Simulator',interval=0, parameterSetters = []).start(func=[init,draw,step])
# 'title', 'interval' and 'parameterSetters' are optional
