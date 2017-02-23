/*
Implementation of the Cross Entropy (CE) algorithm. 
*/

#include <iostream>
#include <vector>
// #include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h"
#include "Algorithm.h"
#include "CrossEntropy.h"

namespace optimization{

	/* Execute Cross Entropy (CE) algorithm for one iteration.
	   Input particles X are directly updated to avoid defining extra variables. */
	MatrixXd& CrossEntropy::run(MatrixXd &X, int TI, double dt, std::default_random_engine &generator){

		// if (TI % 1 == 0)
		// 	{std::cout << "TI = " << TI << '\n';}

		// Evaluate objective function at particles
		RowVectorXd h = obj_fn->evaluate(X); 

		// Evaluate performance of current iteration
		performance(X, h, TI);

		// Select elite samples
		MatrixXd elites = MatrixXd::Zero(X.rows(), num_elites_);
		selectElites(X, h, elites);

		// Compute importance weights
		RowVectorXd weights = RowVectorXd::Constant(num_elites_, 1.0/num_elites_);

		// Update model parameters
		updateParameters(elites, weights);

		// Generate new samples from updated model
		generateNewSamples(X, generator);

		return X;

	}


	/* Select eleite samples according to a quantile rule */
	void CrossEntropy::selectElites(const MatrixXd &X, RowVectorXd &h, MatrixXd &elites){

		// Sort h
		std::vector<double> v;
		v.resize(h.size());
		RowVectorXd::Map(&v[0], h.size()) = h;
		sort(v.begin(), v.begin()+h.size());
		double level = v[num_elites_]; 

		// Select elite samples
		int elite_count = 0;
		for (int i=0; i<h.size(); ++i){
			if (h(i) <= level && elite_count < num_elites_){
				elites.col(elite_count) = X.col(i);
				elite_count += 1;
			}
		}
	}


	/* Use the elite samples to update model parameters */
	void CrossEntropy::updateParameters(MatrixXd &elites, RowVectorXd &weights){
		// Compute weighted mean and covariance
		VectorXd mean_update = VectorXd::Zero(dim);
		MatrixXd cov_update = MatrixXd::Zero(dim, dim);
		for (int i=0; i<num_elites_; ++i){
			mean_update += elites.col(i)*weights(i);
		}
		for (int i=0; i<num_elites_; ++i){
			VectorXd diff = elites.col(i) - mean_update;
			cov_update += weights(i)*diff*(diff.transpose());
		}

		model_mean_ = smoothing_*mean_update + (1-smoothing_)*model_mean_;
		model_cov_ = smoothing_*cov_update + (1-smoothing_)*model_cov_;
	}


	/* Generate new sample from the updated (Gaussian) model. 
	   New samples are stored in X */
	void CrossEntropy::generateNewSamples(MatrixXd &X, std::default_random_engine &generator){
		// Currently neglecting correlations in the updated Gaussian model

		for (int d=0; d<dim; d++){
			std::normal_distribution<double> normal(model_mean_(d), std::sqrt(model_cov_(d,d)));
			RowVectorXd particles_d(num_particles);
			for (int i=0; i<num_particles; i++){
				particles_d(i) = normal(generator);
			}
			X.row(d) = particles_d;
		}
	}


}


