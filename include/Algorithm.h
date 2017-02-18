/*
This header defines the base class of all algorithms. 
*/

#ifndef ALGORITHM
#define ALGORITHM

#include <iostream>
#include <vector>
#include <random>

#include <Eigen/Dense>

// using namespace std;

namespace optimization{

	using namespace Eigen;

	class Obj_fn;

	/* Base class for all algorithms */
	class Algorithm
	{	

	protected:

		Obj_fn *obj_fn;
		int dim;
		int num_particles;

		// Performace metric
		RowVectorXd h_mean;
		RowVectorXd h_best;
		RowVectorXd distance;

	public:

		/* Default trivial constructor */
		Algorithm() {}

		/* Constructor */
		explicit Algorithm(int num_iterations, int dim, int num_particles)
			: dim(dim), num_particles(num_particles) {
			h_mean = RowVectorXd::Zero(num_iterations+1);
			h_best = RowVectorXd::Zero(num_iterations+1);
			distance = RowVectorXd::Zero(num_iterations+1);
		}

		/* Link to the objective function */
		void add_obj(Obj_fn *fn){obj_fn = fn;}

		/* Run the algorithm for one iteration */
		virtual MatrixXd run(const MatrixXd &X, int TI, double dt, std::default_random_engine &generator);

		/* Calculate performance metric of the algorithm, calculated at each iteration
		   This function is used in all algorithms, hence defined in the base class */
		void performance(const MatrixXd &X, const RowVectorXd &h, int TI);

		/* Gather performance measures by returning the metric values */
		std::vector<RowVectorXd> getPerformance();

		/* Print performance measures without returning the metric values */
		void printPerformance(std::string optimizer_name, int which_metric);

	};


} // End of namespace optimization






#endif