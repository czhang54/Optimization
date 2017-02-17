/* This is a simulation tool of global optimization algorithms.  
One example is the controlled particle filter (CPF) algorithm that I have recently worked on. 
The research paper can be found on arXiv: https://arxiv.org/abs/1701.02413. 

The code structure is flexible for the user to add:
(1) objective functions to be minimized (see obj_map below and also Obj_fn.h)
(2) optimization algorithms (Algorithm.h)
(3) initialization methods (Prior.h)
Hence, the project can be easily expanded to accommodate more options. 
*/

#include <iostream>
#include <string>
// #include <vector>
// #include <cmath>
#include <map>

#include <Eigen/Dense>

#include "Obj_fn.h"
#include "Prior.h"
#include "Optimizer.h"
#include "Algorithm.h"
#include "CrossEntropy.h"
#include "ControlledParticleFilter.h"

// using namespace std;
// using namespace Eigen;
// using namespace optimization;

template<typename Obj_type> 
optimization::Obj_fn* define_objective_function(int dim) { return new Obj_type(dim); }

typedef std::map<std::string, optimization::Obj_fn*> Obj_fn_map;

/* Some metric to assess the algorithms */
enum Performance_metric
{
	H_MEAN,
	H_BEST,
	DISTANCE
};



int main(){

	/* Set simulation parameters */
	const double stop = 0.010; // Time length of the simulation
	const double step_size  = 0.001; // Step size of the simulation
	int num_iterations = static_cast<int>(stop/step_size); // Total number of iterations in the simulation
	const int dim = 2; // Dimension of the optimization problem
	const int num_particles = 100; // Used for particle-based methods; For others, num_particles = 1

	/* Define a random number generator. 
	It is initialized only once and used everywhere in the project. 
	It needs to be seeded avoid repeating numbers */
	std::default_random_engine generator;
	generator.seed(static_cast<unsigned int>(std::time(0)));

	/* Pre-define a map from string to an objective function object, 
	so that the user only need to provide the name (string) of the objective function from a given list
	*/
	Obj_fn_map obj_map;
	obj_map["Rastrigin"] = define_objective_function<optimization::Rastrigin>(dim);
	obj_map["Quadratic"] = define_objective_function<optimization::Quadratic>(dim);
	// Can continue to add more functions to expand the map

	/* Define objective function using the map */
	const std::string obj_name("Rastrigin"); 
	/* The function name can be simply made a user input:
	std::cout << "Choose a function from the map above: " << std::endl;
	std::string obj_name;
	std::cin >> obj_name; 
	*/

	optimization::Obj_fn *obj_fn = obj_map[obj_name];
	std::cout << obj_fn;

	/* Define and initialize optimizers */
	// Controlled Particle Filter (CPF) algorithm
	optimization::Optimizer<optimization::Gaussian, optimization::ControlledParticleFilter> optimizer_CPF("CPF", obj_fn, num_iterations, step_size, dim, num_particles);
	optimizer_CPF.initialize(generator);

	// Cross Entropy (CE) algorithm
	optimization::Optimizer<optimization::Gaussian, optimization::CrossEntropy> optimizer_CE("Cross Entropy", obj_fn, num_iterations, step_size, dim, num_particles);
	optimizer_CE.initialize(generator);

	/* Simulate optimizers */
	optimizer_CPF.simulate(generator);
	optimizer_CE.simulate(generator);

	/* Investigate performance using three metrics:
	(1) Average function value, H_MEAN, check how it changes over iterations
	(2) Best function value found, H_BEST, check how it changes over iterations
	(3) Distance from the current state to global minimizer, DISTANCE, check how it changes over iterations */
	std::cout << "Performance of CPF" << '\n';
	std::cout << optimizer_CPF.getPerformance()[H_MEAN] << '\n';
	std::cout << optimizer_CPF.getPerformance()[H_BEST] << '\n';
	std::cout << optimizer_CPF.getPerformance()[DISTANCE] << '\n';

	std::cout << "Performance of CE" << '\n';
	std::cout << optimizer_CE.getPerformance()[H_MEAN] << '\n';
	std::cout << optimizer_CE.getPerformance()[H_BEST] << '\n';
	std::cout << optimizer_CE.getPerformance()[DISTANCE] << '\n';


	delete obj_fn;




	return 0;
}


