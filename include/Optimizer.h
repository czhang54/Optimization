#ifndef OPTIMIZER_TEMPLATE
#define OPTIMIZER_TEMPLATE

#include <iostream>

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
		Optimizer(std::string optimizer_name, const int T, double step, const int d, const int N=1) {
			std::cout << "Defining optimizer..." << '\n';
			name = optimizer_name;
			num_iterations = T;
			step_size = step;
			dim = d;
			num_particles = N;
			state = Eigen::MatrixXd::Zero(dim, num_particles);

			prior = Prior_model(dim);
			algorithm = Algorithm_model(num_iterations);
		}

		/* Trivial destructor */
		~Optimizer(){}

		/* Link to the objective function */
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

		/* Access to the state of the optimizer */
		Eigen::MatrixXd& getState() {return state;}

		/* Access to the perforamance of the optimizer
		   (1) Average function value after each iteration
		   (2) Best function value found after each iteration
		   (3) Distance from the global minimizer */
		std::vector<Eigen::RowVectorXd> getPerformance() {
			return algorithm.getPerformance();
		}


		void message(){
			name << " will be simulated" << '\n';
		}



	};

} // End of namespace optimization




#endif 