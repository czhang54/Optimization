#ifndef WORLD
#define WORLD

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h" // Need to include Obj_fn.h
#include "Optimizer.h"

using namespace std; 
using namespace Eigen;

class Optimizer; // Need to declare Optimizer class to initialize World object


class World
{
	RowVectorXd time;
	int dim;
	int num_times;
	Obj_fn *obj_fn;
	vector<Optimizer*> optimizer_list; // Need declaration of Optimizer class as above
	

public:
	// Need start, stop, step, dim to start
	World(double start, double stop, double step, int d){
		dim = d;
		num_times = static_cast<int>((stop-start)/step) + 1;
		RowVectorXd Time = RowVectorXd::LinSpaced(num_times, start, stop);
		time = Time;
		cout << num_times << '\n';
	}

	int get_time() const {return num_times;}
	int get_dim() const {return dim;}
	Obj_fn* get_obj() const {return obj_fn;}

	// Overload << operator
	friend ostream& operator<<(ostream &out, World &world){
		out << "Time length: " << world.num_times << "  Dimension: " << world.dim << '\n';
		return out;
	}

	// Add objective function
	void add_obj(Obj_fn *fn){obj_fn = fn;}


	// Add optimizers
	void add_optimizer(Optimizer *o){
		optimizer_list.push_back(o);
		o->world = this;
		// cout << "Optimizer added" << '\n';
	}


	// Simulate the optimizers in the list
	void simulate(){
		cout << "Now simulate..." << '\n';
		for (int TI=0; TI<num_times-1; ++TI){
			for (Optimizer* o: optimizer_list){
				o->update(TI, time[TI+1]-time[TI]);
			}
		}
	}
	
};
