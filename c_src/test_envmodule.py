import environment, toml, pdb, objgraph

with open("../../../config.toml") as conffile:
    config = toml.loads(conffile.read())

print config
env = environment.Environment(config)

for i in range(1000):
    env.step()
    if i % 100 == 0:
        pdb.set_trace()
