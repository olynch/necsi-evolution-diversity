#include "environment.h"
#include "ctoml/include/toml.h"
#include <string>
#include <iostream>

using namespace std;
using namespace ctoml;

void printStage(string stage) {
	cout << endl << "********* " << stage << " **********" << endl << endl;
}

int main() {
	printStage("Acquiring Config");
	TomlParser cfg_parser ("config.toml");
	TomlDocument cfg = cfg_parser.parse();
	cfg.set("dist.Empty", TomlValue::create_float(1 - (cfg.get_as<float>("dist.Prey1") + cfg.get_as<float>("dist.Prey2") + cfg.get_as<float>("dist.Predator"))));

	printStage("Testing Square");

	printStage("Testing Direct Constructor");
	Square test_prey1 (PREY1);
	cout << "test_prey1.kind: " << test_prey1.kind << endl;
	cout << "test_prey1 == PREY1: " << (test_prey1 == PREY1) << endl;
	Square test_predator (PREDATOR, 0.26, 0.7);
	cout << "test_predator.kind: " << test_predator.kind << endl;
	cout << "test_predator.repRate: " << test_predator.repRate << endl;
	cout << "test_predator.eats: " << test_predator.eats << endl;
	cout << "test_predator == PREDATOR: " << (test_predator == PREDATOR) << endl;
	
	printStage("Testing Statistical Constructor");
	random_device rand;
	default_random_engine eng (rand());
	uniform_real_distribution<float> rd (0.0, 1.0);
	for (int i = 0; i < 10; ++i) {
		Square test (cfg, rd(eng), rd(eng), rd(eng));
		cout << "kind: " << test.kind << " repRate: " << test.repRate << " eats: " << test.eats << endl;
		cout << "color: {";
		vector<float> rgb_vals = test.rgb();
		for (int j = 0; j < 3; ++j) {
			cout << rgb_vals[j] << ",";
		}
		cout << "}" << endl;
	}

	printStage("Testing shared_ptr swap unique_ptr<env_mtx>");

	unique_ptr<env_mtx> s1 (new env_mtx(boost::extents[100][100]));
	unique_ptr<env_mtx> s2 (new env_mtx(boost::extents[100][100]));
	env_mtx env1 (boost::extents[100][100]);
	env_mtx env2 (boost::extents[100][100]);

	(*s1)[0][0] = shared_ptr<Square>(new Square(PREY1));
	(*s2)[0][0] = shared_ptr<Square>(new Square(PREY2));
	env1[0][0] = shared_ptr<Square>(new Square(PREY1));
	env2[0][0] = shared_ptr<Square>(new Square(PREY2));

	cout << "s1 kind: " << (*s1)[0][0]->kind << endl << "s2 kind: " << (*s2)[0][0]->kind << endl;
	cout << "env1 kind: " << env1[0][0]->kind << endl << "env2 kind: " << env2[0][0]->kind << endl;

	swap(s1, s2);
	swap(env1, env2);
	cout << "Swapped" << endl;

	cout << "s1 kind: " << (*s1)[0][0]->kind << endl << "s2 kind: " << (*s2)[0][0]->kind << endl;
	cout << "env1 kind: " << env1[0][0]->kind << endl << "env2 kind: " << env2[0][0]->kind << endl;


	printStage("Finished Testing Square");
	
	printStage("Testing Environment");

	printStage("Testing Constructor");

	cout << "size: " << cfg.get_as<int>("size") << endl;
	Environment* test_env = new Environment(cfg.get_as<int>("size"), cfg);

	printStage("Testing Refresh");

	for (int i = 0; i < 10; ++i) {
		cout << "kind: " << test_env->get(i, 0)->kind << endl;
	}

	test_env->refresh();
	cout << "refreshed" << endl;

	for (int i = 0; i < 10; ++i) {
		cout << "kind: " << test_env->get(i, 0)->kind << endl;
	}

	test_env->step();
}
