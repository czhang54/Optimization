#ifndef OPTIMIZER
#define OPTIMIZER

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

// #include "World.h"
// #include "Controller.h"

using namespace std;
using namespace Eigen;

// class World;
class World;
class Prior; // Need to declare Prior class to initialize Optimizer object
class Controller;

// No need to declare other dependency class or include .h files, they are declared/included in Optimizer.cpp

typedef Matrix<MatrixXd, Dynamic, 1> Tensor3Xd; // 3d array (tensor)

class Optimizer
{
	int N;
	string name;
	// vector<vector<vector<double>>> value;
	Tensor3Xd value;
	Prior *prior;
	Controller *controller;
	World *world;


public:

	Optimizer(const World *w, const int num_particles, string optimizer_name);

	friend ostream& operator<<(ostream &out, const Optimizer &optimizer);

	Tensor3Xd getValue() const;
	string getName() const;
	World* get_world() const;

	// Set prior model
	void setPrior(Prior *p);

	// Initialize
	void initialize();

	// Set controller model
	void setController(Controller *c);

	void update(int TI, double dt);

	// friend void World::add_optimizer(Optimizer *o);
	friend World; // Must include the entire class, even if we only want to friend with a single member function

	
};

















#endif