
/* This is an example program to simulate various global optimization algorithms. 
The purpose is to illustrate how to use the algorithms and assess their performance. 
Algorithms include: 
  + Simulated annealing (SA)
  + Cross entropy (CE)
  + Particle filter (PF)
  + Controlled particle filter (CPF)
The controlled particle filter (CPF) algorithm is recently propose in my paper  
which can be found on arXiv: https://arxiv.org/abs/1701.02413. 

The entire project is expandable so that the user can add more:
(1) objective functions to be minimized (see obj_map below and also src/Obj_fn.cpp)
(2) optimization algorithms (scr/Algorithm.cpp)
(3) initialization methods (src/Prior.cpp)
*/

#include <iostream>
#include <string>
#include <map>

#include <Eigen/Dense>

#include "Obj_fn.h"
#include "Prior.h"
#include "Optimizer.h"
#include "Algorithm.h"
#include "SimulatedAnnealing.h"
#include "CrossEntropy.h"
#include "ParticleFilter.h"
#include "ControlledParticleFilter.h"
#include "PerformanceMetric.h"

// using namespace std;
// using namespace Eigen;
// using namespace optimization;

template<typename Obj_type> 
optimization::Obj_fn* define_objective_function(int dim) { return new Obj_type(dim); }

typedef std::map<std::string, optimization::Obj_fn*> Obj_fn_map;

int main(){

	/* Set simulation parameters */
	const double stop = 0.020; // Time length of the simulation
	const double step_size  = 0.001; // Step size of the simulation
	int num_iterations = static_cast<int>(stop/step_size); // Total number of iterations in the simulation
	const int dim = 2; // Dimension of the optimization problem
	const int num_particles = 500; // Used for particle-based methods; For others, num_particles = 1

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

	// Simulated Annealing (SA) algorithm
	optimization::Optimizer<optimization::Gaussian, optimization::SimulatedAnnealing> optimizer_SA("SimulatedAnnealing", obj_fn, num_iterations, step_size, dim);
	optimizer_SA.initialize(generator);

	// Cross Entropy (CE) algorithm
	optimization::Optimizer<optimization::Gaussian, optimization::CrossEntropy> optimizer_CE("Cross Entropy", obj_fn, num_iterations, step_size, dim, num_particles);
	optimizer_CE.initialize(generator);

	// Particle Filter (PF) algorithm
	optimization::Optimizer<optimization::Gaussian, optimization::ParticleFilter> optimizer_PF("Particle Filter", obj_fn, num_iterations, step_size, dim, num_particles);
	optimizer_PF.initialize(generator);
	
	// Controlled Particle Filter (CPF) algorithm
	optimization::Optimizer<optimization::Gaussian, optimization::ControlledParticleFilter> optimizer_CPF("Controlled Particle Filter", obj_fn, num_iterations, step_size, dim, num_particles);
	optimizer_CPF.initialize(generator);


	/* Simulate the above optimizers */
	optimizer_SA.simulate(generator);
	optimizer_CE.simulate(generator);
	optimizer_PF.simulate(generator);
	optimizer_CPF.simulate(generator);

	/* Investigate performance using one of the three metrics:
	(1) AVERAGE_VALUE: Average function value at each iterations
	(2) BEST_VALUE_FOUND: Best function value found at each iterations
	(3) DISTANCE_ERROR: Distance between particle mean and the global minimizer at each iterations
	The enum class is defined in "PerformanceMetric.h" */
	optimizer_CPF.printPerformance(BEST_VALUE_FOUND);
	optimizer_CE.printPerformance(BEST_VALUE_FOUND);
	optimizer_CPF.printPerformance(DISTANCE_ERROR);
	optimizer_CE.printPerformance(DISTANCE_ERROR);
	optimizer_PF.printPerformance(DISTANCE_ERROR);
	optimizer_SA.printPerformance(DISTANCE_ERROR);

	delete obj_fn;




	return 0;
}


