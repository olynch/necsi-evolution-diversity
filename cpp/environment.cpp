#include "environment.h"
using namespace std;
using namespace ctoml;

Square::Square(int ikind, float irepRate, float ieats) {
	kind = ikind;
	repRate = irepRate;
	eats = ieats;
}

Square::Square(int ikind) {
	kind = ikind;
	repRate = 0;
	eats = 0;
}

Square::Square() {
	kind = EMPTY;
	repRate = 0;
	eats = 0;
}

Square::Square(const TomlDocument& cfg, float rd1, float rd2, float rd3) {
	if (rd1 <= cfg.get_as<float>("dist.Empty")) {
		kind = 0;
		repRate = 0;
		eats = 0;
	}
	else if (rd1 <= (cfg.get_as<float>("dist.Empty") + cfg.get_as<float>("dist.Prey1"))) {
		kind = 1;
		repRate = 0;
		eats = 0;
	} 
	else if (rd1 <= (cfg.get_as<float>("dist.Empty") + cfg.get_as<float>("dist.Prey1") + cfg.get_as<float>("dist.Prey2"))) {
		kind = 2;
		repRate = 0;
		eats = 0;
	}
	else {
		kind = 3;
		repRate = rd2;
		eats = rd3;
	}
}

Square::Square(shared_ptr<Square> sq) {
	kind = sq->kind;
	repRate = sq->repRate;
	eats = sq->eats;
}

void Square::set(shared_ptr<Square> sq) {
	kind = sq->kind;
	repRate = sq->repRate;
	eats = sq->eats;
}

bool Square::operator==(int other) {
	return (kind == other);
}

bool Square::operator!=(int other) {
	return (kind != other);
}

vector<float> Square::rgb() {
	if (kind == 0) {
		return {1, 1, 1};
	}
	else if (kind == 1) {
		return {0, 1, 0};
	}
	else if (kind == 2) {
		return {0, .5, 0};
	}
	else {
		if (eats == 1) {
			return {1, 0, 0};
		}
		else {
			return {.5, 0, 0};
		}
	}
}

Environment::Environment(int isize, const TomlDocument& cfg) 
	: rd{0.0, 1.0}, data{boost::extents[isize][isize]}, next{boost::extents[isize][isize]}, size{isize}
{
	random_device rand;
	eng = default_random_engine(rand());
	repRateGauss = normal_distribution<float>(0.0, cfg.get_as<float>("mut.repRate"));
	eatsGauss = normal_distribution<float>(0.0, cfg.get_as<float>("mut.eats"));
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			data[i][j].reset(new Square(cfg, rd(eng), rd(eng), rd(eng)));
			next[i][j].reset(new Square(data[i][j]));
		}
	}
}

void Environment::refresh() {
	swap(data, next);
}

shared_ptr<Square> Environment::get(int x, int y) {
	return data[x][y];
}

void Environment::step() {
	bool p1;
	bool p2;
	int numPrey1;
	int numPrey2;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			next[i][j]->set(data[i][j]);

			if (*(data[i][j]) == EMPTY) {
				p1 = false;
				p2 = false;
				numPrey1 = 0;
				numPrey2 = 0;
				for (int k = -1; k <= 1; ++k) {
					for (int l = -1; l <= 1; ++l) {
						if ((k != 0) || (l != 0)) {
							if (*(data[i+k][j+l]) == PREY1) {
								++numPrey1;
							} 
							else if (*(data[i+k][j+l]) == PREY2) {
								++numPrey2;
							}
						}
					}
				}
				if (rd(eng) < (1 - pow(cfg.get_as<float>("repRate.Prey1"), numPrey1))) {
					p1 = true;
				}
				if (rd(eng) < (1 - pow(cfg.get_as<float>("repRate.Prey2"), numPrey2))){
					p2 = true;
				}
				if (p1 && p2) {
					next[i][j]->kind = floor((rd(eng) * 2) + 1);
				}
				else if (p1) {
					next[i][j]->kind = 1;
				}
				else if (p2) {
					next[i][j]->kind = 2;
				}
			}

			else if (*(data[i][j]) == PREY1 || *(data[i][j]) == PREY2) {
				vector<vector<int>> opts;
				for (int k = -1; k <= 1; ++k) { //for all the neighbors
					for (int l = -1; l <=1; ++l) {
						if ((k != 0) || (l != 0)) { //except for self
							if (*(data[i + k][j + l]) == 3 && rd(eng) < data[i + k][j + l]->repRate) { //if it's a predator, and it reproduces
								opts.push_back({i + k, j + l});
							}
						}
					}
				}
				if (opts.size() > 0) {
					int opt = floor(rd(eng) * opts.size());
					next[i][j]->kind = 3;
					next[i][j]->repRate = data[opts[opt][0]][opts[opt][1]]->repRate + repRateGauss(eng);
					next[i][j]->eats = data[opts[opt][0]][opts[opt][1]]->eats + eatsGauss(eng);
				}
			}

			else if (*(data[i][j]) == PREDATOR && rd(eng) < cfg.get_as<float>("deathRate")) {
				next[i][j]->kind = 0;
			}
		}
	}
}
