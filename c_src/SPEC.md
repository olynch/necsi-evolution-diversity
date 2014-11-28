Aim
===
Create a python module written in c containing one class, Environment, implementing something similar to environment.py but much faster.

Spec
====

## Square
A C Struct representing a square of the simulation.

### Fields
- `eats:float`, &gt;=0.5 eats prey2, &lt;0.5 eats prey1
- `kind:char`, 0 is empty, 1 is prey1, 2 is prey2, 3 is predator.
- `repRate:float`, Represents how likely it is to reproduce into a given square at a given timestep.

## Environment
A Python class that simulates a predator-prey model in a cellular automata.

### Variables accessible to python
- `state`, a numpy 3-dimensional ndarray of floats representing the current state of the simulation (row, column, color in RGB) -- replaced at each step
- `stats_maxrep`, a list containing the maximum reproduction rate at each step of the simulation -- appended to at each step
- `stats_minrep`, a list containing the minumum reproduction rate at each step of the simulation -- appended to at each step
- `stats_avgrep`, a list containing the average reproduction rate at each step of the simulation -- appended to at each step
- `stats_eats_data`, a list containing the eats variable for each predator in the simulation -- replaced at each step

### Internal Variables
- `grid`, an array of `Square*`, is actually two-dimensional but is implemented as one-dimensional for efficiency. Length = `size*size`.
- `grid_next`, a buffer for the next step in the simulation
- `size:int`, the length and width of the grid.
- `probabilities`, a struct with values for the initial probability distribution of the grid.

### Methods
- `step()`, runs a step of the simulation.
- `__init__(description:hash)`, 

