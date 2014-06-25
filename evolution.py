import matplotlib
matplotlib.use('TkAgg')

##=====================================
## Section 1: Import Modules
##=====================================

import random as RD
import pylab as PL
import environment as EV

##=====================================
## Section 2: Define Model Parameters
##=====================================

EMPTY, PREY1, PREY2, PREDATOR = 0,1,2,3
size = 10
stepRate = 4
emptyP, predatorP, prey1P, prey2P, bothP = .25, .25, .25, .25, 0
both = False
predatorR, prey1R, prey2R = .2, .2, .2
deathR = .2
repRateM, eatsM = .1, .01

def opeats(e): 1 if e == 2 else 2

##=====================================
## Section 3: Define Three Functions
##=====================================

def init():
    global env
    env = EV.Evnironment(size, {"Empty": emptyP, "Predator": predatorP, "Prey1": prey1P, "Prey2": prey2P})

def draw():
    PL.imshow([[x.kind for x in xs] for xs in env], cmap=plt.cm.earth)

def step():
    global env
    for x in xrange(size):
        for y in xrange(size):
            nbors = env.neighbors(x,y)
            if env[x,y]==EMPTY:
                p1=False
                p2=False
                if RD.random()<1-(1-prey1R)**len(filter((lambda x: x==PREY1), nbors)):
                    p1=True
                    env[x,y] = EV.Square(PREY1)
                if RD.random()<1-(1-prey2R)**len(filter((lambda x: x==PREY2), nbors)):
                    p2=True
                    env[x,y] = EV.Square(PREY2)
                if p1==True and p2 == True:
                    env[x,y] = RD.choice(EV.Square(PREY1),EV.Square(PREY2))
            elif env[x,y]==PREY1 or env[x,y] == PREY2:
                for nbor in filter((lambda x: x==PREDATOR), nbors):
                    if nbor.eats == env[x,y] && RD.random()<nbor.repRate:
                        env[x,y] = EV.Square(PREDATOR, nbor.repRate+RD.gauss(0,repRateM), opeats(nbor.eats) if RD.random()<eatsM else nbor.eats)
            elif env[x,y]==PREDATOR:
                if RD.random()<deathR:
                    env[x,y]=EV.Square(EMPTY)
            else:
                print "environment wasn't a resonable value"
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
