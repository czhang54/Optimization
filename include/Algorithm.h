#ifndef ALGORITHM
#define ALGORITHM

#include <iostream>
#include <vector>

#include <Eigen/Dense>

// using namespace std;

namespace optimization{

	using namespace Eigen;

	class Obj_fn;

	class Algorithm
	{	

	protected:

		Obj_fn *obj_fn;

		// Performace metric
		RowVectorXd h_mean;
		RowVectorXd h_best;
		RowVectorXd distance;

	public:

		/* Default trivial constructor */
		Algorithm() {}

		/* Constructor */
		explicit Algorithm(int num_iterations) {
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

		/* Gather performance measures */
		std::vector<RowVectorXd> getPerformance();

	};


	class CPF: public Algorithm
	{

	public:

		/* Default trivial constructor */
		CPF() {}

		/* Constructor */
		explicit CPF(int num_iterations)
			: Algorithm(num_iterations) {}

		/* Run the algorithm for one iteration */
		virtual MatrixXd run(const MatrixXd &X, int TI, double dt, std::default_random_engine &generator);

		/* A function that calculate control force for each particle at each iteration */
		MatrixXd& Control(const MatrixXd &X, const RowVectorXd &h_diff, MatrixXd &u);

	};


} // End of namespace optimization






#endif
