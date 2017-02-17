/*
Implementation of the Cross Entropy (CE) algorithm. 
*/

#include <iostream>
#include <vector>
#include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h"
#include "Algorithm.h"
#include "CrossEntropy.h"

namespace optimization{

	MatrixXd CrossEntropy::run(const MatrixXd &X, int TI, double dt, std::default_random_engine &generator){

		if (TI % 1 == 0)
			{std::cout << "TI = " << TI << '\n';}

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
		MatrixXd new_samples = MatrixXd::Zero(dim, num_particles);
		generateNewSamples(new_samples, generator);

		return new_samples;

	}


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


	void CrossEntropy::generateNewSamples(MatrixXd &new_samples, std::default_random_engine &generator){
		// Currently neglecting correlations of updated model

		for (int d=0; d<dim; d++){
			std::normal_distribution<double> normal(model_mean_(d), std::sqrt(model_cov_(d,d)));
			RowVectorXd particles_d(num_particles);
			for (int i=0; i<num_particles; i++){
				particles_d(i) = normal(generator);
			}
			new_samples.row(d) = particles_d;
		}
	}


}


