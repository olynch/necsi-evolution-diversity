#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <map>
#include <vector>
#include <functional>
#include <random>
#include <cmath>
#include <memory>
#include "boost/multi_array.hpp"
#include "ctoml/include/toml.h"

const int EMPTY = 0;
const int PREY1 = 1;
const int PREY2 = 2;
const int PREDATOR = 3;

class Square {
	public:
		float eats;
		float repRate;
		int kind;
		Square ();
		Square (int);
		Square (int, float, float);
		Square (const ctoml::TomlDocument&, float, float, float);
		Square (std::shared_ptr<Square>);
		void set(std::shared_ptr<Square>);
		bool operator==(int);
		bool operator!=(int);
		std::vector<float> rgb();
};

typedef boost::multi_array<std::shared_ptr<Square>, 2> env_mtx;

class Environment {
	private:
		env_mtx data;
		env_mtx next;
		std::default_random_engine eng;
		std::uniform_real_distribution<float> rd;
		std::normal_distribution<float> repRateGauss;
		std::normal_distribution<float> eatsGauss;
		ctoml::TomlDocument cfg;
	public:
		int size;
		//boost::multi_array<int, 3> clr_mtx;
		void refresh();
		Environment(int, const ctoml::TomlDocument&);
		std::shared_ptr<Square> get(int, int);
		void step();
};

#endif /* ENVIRONMENT_H_ */
