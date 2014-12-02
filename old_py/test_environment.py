import environment as EV
import random as RD

print("TESTING environment.Square\n")

print("TESTING creation from probability distribution\n")

test_dist = {"Empty": .5, "Prey1": .1, "Prey2": .2, "Predator": .3}

for i in range(0, 10):
	print("Created {0}".format(repr(EV.Square(test_dist))))

print('\n')

print("TESTING creation from provided values\n")

print("Created Empty: {0}".format(repr(EV.Square(0))))
print("Created Prey1: {0}".format(repr(EV.Square(1))))
print("Created Prey2: {0}".format(repr(EV.Square(2))))
print("Created Predator: {0}".format(repr(EV.Square(3, .485, 2))))
print('\n')

print("TESTING environment.Environment\n")

print("TESTING creation from probability distribution\n")

test_env = EV.Environment(10, test_dist)

print("TESTING __getitem__\n")

for i in range(0, 10):
	for j in range(0, 10):
		print("At test_env[{0},{1}] there is {2}".format(i, j, repr(test_env[i,j])))

print("TESTING __setitem__ and refresh()\n")

setValues = []

for i in range(0, 10):
	randx, randy = RD.randrange(10), RD.randrange(10)
	setValues.append((randx,randy))
	newSquare = EV.Square(test_dist)
	test_env[randx, randy] = newSquare
	print("Set test_env[%d,%d] to %s" % (randx, randy, repr(newSquare)))
	print("The value of getting test_env[{0},{1}] is {2}".format(randx, randy, test_env[randx,randy]))

print('\n')
print("RUNNING refresh()")
test_env.refresh()
print('\n')

for i in range(0, 10):
	print("The value of getting test_env[{0},{1}] is {2}".format(setValues[i][0], setValues[i][1], test_env[setValues[i][0],setValues[i][1]]))

print('\n')

print("TESTING neighbors")

for i in range(0, 10):
	randx, randy = RD.randrange(10), RD.randrange(10)
	print("The neighbors of test_env[{0},{1}] are {2}".format(randx, randy, str(test_env.neighbors(randx, randy))))
