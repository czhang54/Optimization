#include <iostream>
#include <string>
// #include <vector>
// #include <cmath>
#include <map>

#include <Eigen/Dense>

#include "Obj_fn.h"
#include "Prior.h"
#include "Optimizer.h"

// using namespace std;
// using namespace Eigen;
// using namespace optimization;

template<typename Obj_type> 
optimization::Obj_fn* define_objective_function(int dim) { return new Obj_type(dim); }

typedef std::map<std::string, optimization::Obj_fn*> Obj_fn_map;



int main(){

	/* Set simulation parameters */
	const double start = 0.0;
	const double stop  = 0.010;
	const double step_size  = 0.001;
	const int dim = 2;
	const int num_particles = 100;

	std::default_random_engine generator;
	generator.seed(static_cast<unsigned int>(std::time(0)));

	Obj_fn_map obj_map;
	obj_map["Rastrigin"] = define_objective_function<optimization::Rastrigin>(dim);
	obj_map["Quadratic"] = define_objective_function<optimization::Quadratic>(dim);

	/* Define objective function */
	const std::string obj_name("Rastrigin");

	optimization::Obj_fn *fn = obj_map[obj_name];
	std::cout << fn;

	/* Define and initialize optimizers */
	optimization::Optimizer<optimization::Gaussian, optimization::CPF> optimizer("CPF", static_cast<int>(stop/step_size), step_size, dim, num_particles);
	optimizer.add_obj(fn);
	optimizer.initialize(generator);
	// std::cout << optimizer.getState() << std::endl; // Print initial particles if needed

	/* Simulate optimizers */
	optimizer.simulate(generator);

	std::cout << optimizer.getPerformance()[0] << '\n';
	std::cout << optimizer.getPerformance()[1] << '\n';
	std::cout << optimizer.getPerformance()[2] << '\n';







	return 0;
}


