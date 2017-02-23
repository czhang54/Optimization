
/* This header defines the Simulated Annealing (SA) algorithm. */

#ifndef SIMULATEDANNEALING
#define SIMULATEDANNEALING

#include <iostream>

#include <Eigen/Dense>

#include <Algorithm.h>


namespace optimization{

	class SimulatedAnnealing: public Algorithm{

		double diffusion_kernel_ = 0.1;

	public:

		/* Trivial constructor */
		SimulatedAnnealing() {}

		/* Constructor */
		SimulatedAnnealing(int num_iterations, double step_size, int dim, int num_particles=1)
			: Algorithm(num_iterations, step_size, dim, num_particles) {}

		virtual MatrixXd& run(MatrixXd &X, int TI, double dt, std::default_random_engine &generator);

	};

}


#endif




