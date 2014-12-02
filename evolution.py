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


##=====================================
## Section 3: Define Three Functions
##=====================================

def init():
    global env, c
    with open("config.toml") as conffile:
        c = toml.loads(conffile.read())

    env = EV.Environment(c)

def draw():
    PL.clf()
    PL.subplot(1, 3, 1)
    PL.imshow(env.state, interpolation = 'nearest')
    PL.subplot(1, 3, 2)
    PL.plot(env.stats_avgrep)
    PL.plot(env.stats_maxrep)
    PL.plot(env.stats_minrep)
    PL.subplot(1, 3, 3)
    PL.hist(env.stats_eats_data)

def step():
    global env
    env.step()

##=====================================
## Section 4: [Optional] Create Setter/Getter Functions for Model Parameters
##=====================================


##=====================================
## Section 5: Import and Run GUI
##=====================================

import pycxsimulator
pycxsimulator.GUI(title='My Simulator',interval=0, parameterSetters = []).start(func=[init,draw,step])
# 'title', 'interval' and 'parameterSetters' are optional
