/*
Implementation of the Algorithm base class. 
*/

#include <iostream>
#include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h" 
// #include "Prior.h"
#include "Optimizer.h"
#include "Algorithm.h" 

// using namespace std;


namespace optimization{

	using namespace Eigen;

    /* Algorithm is the base class for all algorithms */
	MatrixXd Algorithm::run(const MatrixXd &X, int TI, double dt, std::default_random_engine &generator){

		return X;
	}


	void Algorithm::performance(const MatrixXd &X, const RowVectorXd &h, int TI){

		// Calculate h_mean 
		h_mean(TI) = h.mean();

		// Calculate h_best
		double hb = h.minCoeff();
		if (TI == 0){h_best(TI) = hb;}
		else if (hb < h_best(TI-1)){h_best(TI) = hb;}
		else {h_best(TI) = h_best(TI-1);}

		// Calculate distance between mean of X and global minimizer 
		VectorXd mean = VectorXd::Zero(dim);
		if (num_particles > 1){
			for (int d=0; d<X.rows(); ++d){
				mean(d) = X.row(d).mean();
			}
		}
		else{
			mean = X.col(0);
		}
		VectorXd X_diff = mean.array() - obj_fn->global_minimizer().array();
		distance(TI) = X_diff.norm();

	}

	/* Access to the perforamance of the optimizer
		   (1) Average function value after each iteration
		   (2) Best function value found after each iteration
		   (3) Distance from the global minimizer */
	std::vector<RowVectorXd> Algorithm::getPerformance(){
		std::vector<RowVectorXd> p;
		p.push_back(h_mean);
		p.push_back(h_best);
		p.push_back(distance);
		return p;
	}
	

} // End of namespace optimization


