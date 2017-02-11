#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#include <Eigen/Dense>

#include "World.h"
#include "Obj_fn.h"
#include "Optimizer.h"
#include "Prior.h"
#include "Controller.h"

using namespace std;
using namespace Eigen;
// using namespace optimization;

template<typename T> Obj_fn* def_objective_function(int dim) { return new T(dim); }

typedef map<string, Obj_fn*> Obj_fn_map;



int main(){

	/* Fix important parameters */
	const double start = 0.0;
	const double stop  = 0.100;
	const double step  = 0.001;
	const int dim = 2;
	const int num_particles = 200;
	
	// Initial distribution (Gaussian)
	const string init_dist("gaussian");
	const VectorXd init_mean = VectorXd::Constant(dim, 40);
	const VectorXd init_std = VectorXd::Constant(dim, 20); // Assume uncorrelated components
	// Initial distribution (Uniform)
	const VectorXd l_range = VectorXd::Constant(dim, -50);
	const VectorXd r_range = VectorXd::Constant(dim, 50);

	const string optimizer_name ("CPF"); // Optional

	/* Initialize the world */
	World world(start, stop, step, dim);
	cout << world;

	Obj_fn_map obj_map;
	obj_map["Rastrigin"] = def_objective_function<Rastrigin>(dim);
	obj_map["Quadratic"] = def_objective_function<Quadratic>(dim);

	/* Define objective function */
	const string obj_name("Quadratic");
	
	// Rastrigin rastrigin(dim);
	// Obj_fn *fn = &rastrigin;

	Obj_fn *fn = obj_map[obj_name];
	cout << fn;

	world.add_obj(fn);
	cout << world.get_obj();

	/* Define Prior model */
	// Need to use kwargs to create a map!


	Gaussian gaussian(init_mean, init_std);
	Prior *prior = &gaussian;

	prior->message(); // Print name of initial distribution


	/* Define Controller model */
	CPF cpf(world.get_time());
	Controller *controller = &cpf;


	/* Define optimizer */
	Optimizer optimizer_CPF(&world, num_particles, optimizer_name);
	cout << optimizer_CPF;

	world.add_optimizer(&optimizer_CPF);

	// Set prior model of optimizer
	optimizer_CPF.setPrior(prior);
	optimizer_CPF.initialize();

	// Set controller model of optimizer
	// optimizer_CPF.setController(controller);
	optimizer_CPF.setController(&cpf);
	

	/* Simulate */
	world.simulate();

	// Assess performance
	vector<RowVectorXd> performance = controller->performance();
	RowVectorXd h_hat = performance[0];
	cout << h_hat << '\n';
	// cout << h_hat(world.get_time()-1) << '\n';


	delete fn;
	fn = nullptr;

	return 0;
}


