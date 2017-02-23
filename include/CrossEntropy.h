/*
This header defines the Cross Entropy (CE) algorithm. 
*/

#ifndef CROSSENTROPY
#define CROSSENTROPY

#include <iostream>

#include <Eigen/Dense>

#include <Algorithm.h>


namespace optimization{

    class CrossEntropy: public Algorithm
	{
		double quantile_  = 0.1; // Quantile parameter to select elite samples
		double smoothing_ = 0.2; // Smoothing parameter
		int num_elites_; // Number of elite samples (fixed)
		VectorXd model_mean_; // Mean of the model, updated at each iteration
		MatrixXd model_cov_; // Covariance matrix of the model, updated at each iteration

	public:

		/* Trivial constructor */
		CrossEntropy() {}

		/* Constructor */
		CrossEntropy(int num_iterations, double step_size, int dim, int num_particles) 
			: Algorithm(num_iterations, step_size, dim, num_particles) {
				num_elites_ = static_cast<int>(num_particles*quantile_);
				model_mean_ = VectorXd::Constant(dim, 40);
				VectorXd cov = VectorXd::Constant(dim, 400);
				model_cov_ = cov.asDiagonal();
			}

		/* Run the algorithm for one iteration */
		virtual MatrixXd& run(MatrixXd &X, int TI, double dt, std::default_random_engine &generator);

		/* Select eleite samples according to a quantile rule */
		void selectElites(const MatrixXd &X, RowVectorXd &h, MatrixXd &elites); 

		/* Use the elite samples to update model parameters */
		void updateParameters(MatrixXd &elites, RowVectorXd &weights);

		/* Generate new sample from the updated model */
		void generateNewSamples(MatrixXd &X, std::default_random_engine &generator);



	};

} // End of namespace optimization


#endif


