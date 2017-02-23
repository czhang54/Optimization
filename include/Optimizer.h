/* 
This header file provides definitions of the Optimizer class. 
Any optimizer needs two basic components:
	(1) Initialization (Prior): For particle-based methods, need to sample from a prior distribution; 
	                            For non-particle methods, just need a point, either random or fixed
	(2) Algorithm: To update the optimizer at each iteration. 
These two components are user-defined and distinguish optimizers. 
Hence, the Optimizer class is a template with Prior (P) and Algorithm (A) as two template parameters. 
Specific Prior and Algorithm models are defined in Prior.h and Algorithm.h, respectively.
*/


#ifndef OPTIMIZER
#define OPTIMIZER

#include <iostream>
#include <string>

#include <Eigen/Dense>

/* In order for the compiler to use a template, it must see both the template definition (not just a declaration) 
and the template type used to instantiate the template. 
The following headers MUST be included since they appear in template type parameter for the Optimizer class */
#include "Prior.h"
#include "Algorithm.h"

// using namespace std;
// using namespace Eigen;

namespace optimization{

	class Obj_fn;
	// class Prior;
	// class Algorithm;

	template <class P, class A>
	class Optimizer{

		typedef P Prior_model;
		typedef A Algorithm_model;

		std::string name; // Name of the optimizer
		int num_iterations; // Number of iterations
		double step_size; // Step size of the algorithm. Constant value by default
		int num_particles; // Number of particles. N=1 for algorithms without particles e.g. SA
		int dim; // Dimension of the state
		Eigen::MatrixXd state; // State of the optimizer, updated after each iteration

		Obj_fn *obj_fn; // Objective function object
		Prior_model prior; // Prior model object
		Algorithm_model algorithm; // Algorithm model object

	public:

		/* Constructor */
		Optimizer(std::string optimizer_name, Obj_fn *fn, const int T, double step, const int d, const int N=1) {
			std::cout << "Defining optimizer..." << '\n';
			name = optimizer_name;
			num_iterations = T;
			step_size = step;
			dim = d;
			num_particles = N;
			state = Eigen::MatrixXd::Zero(dim, num_particles);

			prior = Prior_model(dim);
			algorithm = Algorithm_model(num_iterations, step_size, dim, num_particles);
			obj_fn = fn;
		}

		/* Trivial destructor */
		~Optimizer(){}

		/* Link to the objective function (not necessary) */
		void add_obj(Obj_fn *fn){obj_fn = fn;}

		/* Initialize the optimizer:
		   (1) Generate initial samples
		   (2) Link the objective function to the algorithm */
		void initialize(std::default_random_engine &generator) {
			// Prior *prior = new Prior(d); // Define prior model of type Prior
			std::cout << "initializing optimizer..." << '\n';
			state = prior.sample_IC(num_particles, generator);
			algorithm.add_obj(obj_fn);
			// delete prior;
		}

		/* Simulate the optimizer by running its algorithm */
		void simulate(std::default_random_engine &generator){
			for (int TI=0; TI<num_iterations; ++TI){
				state = algorithm.run(state, TI, step_size, generator); // Execute one iteration of the algorithm
			}	
		}

		/* Access to the state of the optimizer (not frequently used) */
		Eigen::MatrixXd& getState() {return state;}

		/* Access to the perforamance of the optimizer by returning:
		   (1) Average function value after each iteration
		   (2) Best function value found after each iteration
		   (3) Distance from the global minimizer */
		std::vector<Eigen::RowVectorXd> getPerformance() {
			return algorithm.getPerformance();
		}

		/* Print the perforamance metric of the optimizer without returning the metric values.
		   Need to declare which metric to print. */
		void printPerformance(int which_metric) {
			algorithm.printPerformance(name, which_metric);
		}


		void message(){
			name << " will be simulated" << '\n';
		}



	};

} // End of namespace optimization




#endif 