import matplotlib
matplotlib.use('TkAgg')

##=====================================
## Section 1: Import Modules
##=====================================

import random as RD
import pylab as PL
import environment as EV
import copy

##=====================================
## Section 2: Define Model Parameters
##=====================================

EMPTY, PREY1, PREY2, PREDATOR = 0,1,2,3
size = 10
stepRate = 4
emptyP, predatorP, prey1P, prey2P = .7, .0, .3, .0
both = False
predatorR, prey1R, prey2R = .2, .2, .2
deathR = .2
repRateM, eatsM = .1, .01

whenloop=1


##=====================================
## Section 3: Define Three Functions
##=====================================

def init():
    global env, env_prev
    env = EV.Environment(size, {"Empty": emptyP, "Predator": predatorP, "Prey1": prey1P, "Prey2": prey2P})
    env_prev = copy.deepcopy(env)

def draw():
    #for x in xrange(size):
        #for y in xrange(size):
            #print x,y, env[x,y].kind
    PL.imshow(env.color_repr, interpolation = 'nearest')

def looping():
    for x in xrange(size):
        for y in xrange(size):
            if env._data[x,y] != env_prev._next[x][y]:
                return False
    return True

def opeats(e): PREY1 if e == PREY2 else PREY2

def step():
    global env, env_prev, whenloop
    env_prev=copy.deepcopy(env)
    for x in xrange(size):
        for y in xrange(size):
            env[x,y] = env[x,y]
            nbors = env.neighbors(x,y)
            if env[x,y]==EMPTY:
                print "here!"
                p1=False
                p2=False
                if RD.random()<(1-(1-prey1R)**len(filter((lambda x: x==PREY1), nbors))):
                    p1=True
                    env[x,y] = EV.Square(PREY1)
                if RD.random()<(1-(1-prey2R)**len(filter((lambda x: x==PREY2), nbors))):
                    p2=True
                    env[x,y] = EV.Square(PREY2)
                if p1==True and p2 == True:
                    env[x,y] = EV.Square(RD.choice((PREY1,PREY2)))
            elif env[x,y] == PREY1 or env[x,y] == PREY2:
                for nbor in filter((lambda x: x==PREDATOR), nbors):
                    if nbor.eats == env[x,y] and RD.random()<nbor.repRate:
                        env[x,y] = EV.Square(PREDATOR, nbor.repRate+RD.gauss(0,repRateM), opeats(nbor.eats) if RD.random()<eatsM else nbor.eats)
            elif env[x,y] == PREDATOR:
                print "GAH"
                if RD.random()<deathR:
                    env[x,y]=EV.Square(EMPTY)
            else:
                print "environment wasn't a resonable value"
    env.refresh()
    if not looping(): whenloop += 1
    print looping()
    print whenloop
    #global env
    #for x in xrange(size):
        #for y in xrange(size):
            #nbors = env.neighbors(x,y)
            #if env[x,y]==EMPTY:
                #p1=False
                #p2=False
                #if RD.random()<(1-(1-prey1R)**len(filter((lambda x: x==PREY1), nbors))):
                    #p1=True
                    #env[x,y] = EV.Square(PREY1)
                #if RD.random()<(1-(1-prey2R)**len(filter((lambda x: x==PREY2), nbors))):
                    #p2=True
                    #env[x,y] = EV.Square(PREY2)
                #if p1==True and p2 == True:
                    #env[x,y] = EV.Square(RD.choice((PREY1,PREY2)))
            #elif env[x,y] == PREY1 or env[x,y] == PREY2:
                #for nbor in filter((lambda x: x==PREDATOR), nbors):
                    #if nbor.eats == env[x,y] and RD.random()<nbor.repRate:
                        #env[x,y] = EV.Square(PREDATOR, nbor.repRate+RD.gauss(0,repRateM), opeats(nbor.eats) if RD.random()<eatsM else nbor.eats)
            #elif env[x,y] == PREDATOR:
                #if RD.random()<deathR:
                    #env[x,y]=EV.Square(EMPTY)
            #else:
                #print "environment wasn't a resonable value"
    #env.refresh()

##=====================================
## Section 4: [Optional] Create Setter/Getter Functions for Model Parameters
##=====================================


##=====================================
## Section 5: Import and Run GUI
##=====================================

import pycxsimulator
pycxsimulator.GUI(title='My Simulator',interval=0, parameterSetters = []).start(func=[init,draw,step])
# 'title', 'interval' and 'parameterSetters' are optional
