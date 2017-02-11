#ifndef WORLD
#define WORLD

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

#include <Eigen/Dense>

using namespace std; 
using namespace Eigen;

class Optimizer; // Need to declare Optimizer class to initialize World object
class Obj_fn;


class World
{
	RowVectorXd time;
	int dim;
	int num_times;
	Obj_fn *obj_fn;
	vector<Optimizer*> optimizer_list; // Need declaration of Optimizer class as above
	

public:
	// Need start, stop, step, dim to start
	World(double start, double stop, double step, int d);

	int get_time() const;
	int get_dim() const;
	Obj_fn* get_obj() const;

	// Overload << operator
	friend ostream& operator<<(ostream &out, World &world);

	// Add objective function
	void add_obj(Obj_fn *fn);

	// Add optimizers
	void add_optimizer(Optimizer *o);

	// Simulate the optimizers in the list
	void simulate();
	
};























#endif