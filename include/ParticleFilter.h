
/*
This header defines the Particle filter (PF) algorithm. 
*/

#ifndef PARTICLEFILTER
#define PARTICLEFILTER

#include <iostream>

#include <Eigen/Dense>

#include "Algorithm.h"

namespace optimization{

	class ParticleFilter: public Algorithm{

		double diffusion_kernel_ = 0.1; // Std. dev. of diffusion noise in each direction

	public:

		/* Trivial constructor */
		ParticleFilter() {}

		/* Constructor */
		ParticleFilter(int num_iterations, double step_size, int dim, int num_particles)
			: Algorithm(num_iterations, step_size, dim, num_particles) {}

		virtual MatrixXd& run(MatrixXd &X, int TI, double dt, std::default_random_engine &generator);

		void resample(MatrixXd &X, const RowVectorXd &weights, std::default_random_engine &generator);

	};






} // End of optimization namespace


#endif
