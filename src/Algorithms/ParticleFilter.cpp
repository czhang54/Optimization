
/*
Implementation of the Particle filter (PF) algorithm. 
*/

#include <iostream>
// #include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h"
#include "Algorithm.h"
#include "ParticleFilter.h"

namespace optimization{

	/* Execute Particle filter (PF) algorithm for one iteration. 
	   Input particles X are directly updated to avoid defining extra variables. */
	MatrixXd& ParticleFilter::run(MatrixXd &X, int TI, double dt, std::default_random_engine &generator){

		// if (TI % 1 == 0)
		// 	{std::cout << "TI = " << TI << '\n';}

		// Evaluate objective function at particles
		RowVectorXd h = obj_fn->evaluate(X);

		// Evaluate performance of current iteration
		performance(X, h, TI);

		// Calculate importance weights for particles
		RowVectorXd weights = (-step_size*h).array().exp(); 
		double sum = weights.sum();
		weights /= sum; // Normalize the weights

		// Resampling		
		resample(X, weights, generator);

		// Diffuse particles to increase search range
		for (int i=0; i<num_particles; i++){	
			for (int d=0; d<dim; d++){
				std::normal_distribution<double> normal(0, diffusion_kernel_);
				X(d,i) += normal(generator);
			}
		}		

		return X;

	}

	/* Resampling with replacement. 
	   Input X is replaced by resampled particles. */
	void ParticleFilter::resample(MatrixXd &X, const RowVectorXd &weights, std::default_random_engine &generator){		

		MatrixXd resampled_particles = MatrixXd::Zero(dim, num_particles);

		std::uniform_real_distribution<double> uniform(0, 1/(static_cast<double>(num_particles)));
		double r = uniform(generator);

		double c = weights(0);
		int count = 0;

		for (int n=0; n<num_particles; ++n){
			double U = r + n/(static_cast<double>(num_particles));
			while (U > c){
				count += 1;
				c += weights(count);
			}
			resampled_particles.col(n) = X.col(count);
		}

		X = resampled_particles;
		
	}

} // End of optimization namespace


