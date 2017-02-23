
/*
Implementation of the Simulated Annealing (SA) algorithm. 
*/

#include <iostream>
// #include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h"
#include "Algorithm.h"
#include "SimulatedAnnealing.h"

namespace optimization{

	/* Execute Particle filter (PF) algorithm for one iteration. 
	   Input state X is directly updated to avoid defining extra variables. 
	   Note that X has only ONE column */
	MatrixXd& SimulatedAnnealing::run(MatrixXd &X, int TI, double dt, std::default_random_engine &generator){

		RowVectorXd h = obj_fn->evaluate(X);

		// Evaluate performance of current iteration
		performance(X, h, TI);

		// Obtain a candidate solution bu diffusing X
		MatrixXd candidate = MatrixXd::Zero(dim, 1);
		for (int d=0; d<dim; d++){
			std::normal_distribution<double> normal(0, diffusion_kernel_);
			candidate(d) = X(d) + normal(generator);
		}

		// Evaluate function value at the candidate
		RowVectorXd h_candidate = obj_fn->evaluate(X);

		// Select/reject the candidate solution
		double temperature = fabs(h_best(TI))/log(TI+2); // Temperature according to a cooling schedule
		double Metropolis_ratio = exp((h(0)-h_candidate(0))/temperature); 
		double accept_probability = fmin(Metropolis_ratio, 1.0);
		std::uniform_real_distribution<double> uniform(0, 1);
		double v = uniform(generator);
		if (v < accept_probability){X = candidate;}

		return X;

	}

} // End of optimization namespace


