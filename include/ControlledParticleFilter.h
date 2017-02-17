/*
This header defines the Controlled Particle Filter (CPF) algorithm. 
*/

#include <iostream>
// #include <random>

#include <Eigen/Dense>

#include "Algorithm.h"

namespace optimization{

	class ControlledParticleFilter: public Algorithm {

	public:

		/* Default trivial constructor */
		ControlledParticleFilter() {}

		/* Constructor */
		explicit ControlledParticleFilter(int num_iterations, int dim, int num_particles)
			: Algorithm(num_iterations, dim, num_particles) {}

		/* Run the algorithm for one iteration */
		virtual MatrixXd run(const MatrixXd &X, int TI, double dt, std::default_random_engine &generator);

		/* A function that calculate control force for each particle at each iteration */
		MatrixXd& affine_control(const MatrixXd &X, const RowVectorXd &h_diff, MatrixXd &u);

	};


} // End of namespace optimization