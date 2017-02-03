#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#include <Eigen/Dense>

#include "../include/World.h"
#include "../include/Obj_fn.h"
#include "../include/Optimizer.h"
#include "../include/Prior.h"
#include "../include/Controller.h"

using namespace std;
using namespace Eigen;

template<typename T> Obj_fn* createInstance(int dim) { return new T(dim); }

typedef map<string, Obj_fn*> map_type;



int main(){

	/* Fix important parameters */
	const double start = 0.0;
	const double stop  = 0.500;
	const double step  = 0.001;
	const int dim = 2;
	const int num_particles = 200;
	
	// Initial distribution (Gaussian)
	const string init_dist("gaussian");
	const VectorXd mean = VectorXd::Constant(dim, 40);
	const VectorXd std = VectorXd::Constant(dim, 20); // Assume uncorrelated components
	// Initial distribution (Uniform)
	const VectorXd l_range = VectorXd::Constant(dim, -50);
	const VectorXd r_range = VectorXd::Constant(dim, 50);

	const string optimizer_name ("CPF"); // Optional

	/* Initialize the world */
	World world(start, stop, step, dim);
	cout << world;

	map_type map_obj;
	map_obj["Rastrigin"] = createInstance<Rastrigin>(dim);
	map_obj["Quadratic"] = createInstance<Quadratic>(dim);

	/* Define objective function */
	const string obj_name("Quadratic");
	
	// Rastrigin rastrigin(dim);
	// Obj_fn *fn = &rastrigin;

	Obj_fn *fn = map_obj[obj_name];
	cout << fn;

	world.add_obj(fn);
	cout << world.get_obj();

	/* Define Prior model */
	// Need to use kwargs to create a map!


	Gaussian gaussian(mean, std);
	Prior *prior = &gaussian;

	prior->message(); // Print name of initial distribution


	/* Define Controller model */
	CPF cpf(world.get_time());
	Controller *controller = &cpf;


	/* Define optimizer */
	Optimizer optimizer(&world, num_particles, optimizer_name);
	cout << optimizer;

	world.add_optimizer(&optimizer);

	// Set prior model of optimizer
	optimizer.setPrior(prior);
	optimizer.initialize();

	// Set controller model of optimizer
	optimizer.setController(controller);
	// optimizer.setController(&cpf);
	

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


