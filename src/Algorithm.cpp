#include <iostream>
#include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h" 
#include "Prior.h"
#include "Optimizer.h"
#include "Algorithm.h" 

// using namespace std;


namespace optimization{

	using namespace Eigen;

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
		VectorXd mean = VectorXd::Zero(X.rows());
		if (X.cols() > 1){
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


	std::vector<RowVectorXd> Algorithm::getPerformance(){
		std::vector<RowVectorXd> p;
		p.push_back(h_mean);
		p.push_back(h_best);
		p.push_back(distance);
		return p;
	}


	MatrixXd CPF::run(const MatrixXd &X, int TI, double dt, std::default_random_engine &generator) {

		if (TI % 1 == 0)
			{std::cout << "TI = " << TI << '\n';}

		RowVectorXd h = obj_fn->evaluate(X); // Evaluate objective function at particles

		performance(X, h, TI);
		
		// std::cout << "h_mean = " << h_mean(TI) << '\n';
		// std::cout << "h_best = " << h_best(TI) << '\n';
		// std::cout << "distance = " << distance(TI) << '\n';
		
		RowVectorXd h_diff = h.array() - h_mean(TI);

		// Main part: calculate control function for each particle
		MatrixXd u = MatrixXd::Zero(X.rows(), X.cols());
		u = Control(X, -h_diff, u);

		return X + u*dt;
	}


	// Affine control law
	MatrixXd& CPF::Control(const MatrixXd &X, const RowVectorXd &h_diff, MatrixXd &u){

		typedef Matrix<MatrixXd, Dynamic, 1> Tensor3Xd; // 3d array (tensor)

		int dim = X.rows();
		int N   = X.cols();
		int L   = dim*(dim+1)/2;

		// MatrixXd  u = MatrixXd::Zero(dim, N);
		MatrixXd  K = MatrixXd::Zero(dim, N);
		VectorXd  B = VectorXd::Zero(dim);
		VectorXd  X_mean = VectorXd::Zero(dim);
		MatrixXd  Phi = MatrixXd::Zero(L, N);
		Tensor3Xd gradPhi(dim);

		/* Compute B */
		for (int d=0; d<dim; ++d){
			B(d) = ((h_diff.array()*(X.row(d).array())).matrix()).mean();
			X_mean(d) = X.row(d).mean();
			gradPhi(d) = MatrixXd::Zero(L, N);
		}

		/* Compute K */	
		// Evaluate basis functions
		int count(0);
		for (int d=0; d<dim; ++d){
			Phi.row(count) = ((X.row(d).array() - X_mean(d))*(X.row(d).array() - X_mean(d))).matrix();
			gradPhi(d).row(count) = 2*(X.row(d).array() - X_mean(d)).matrix();
			count += 1;
		}

		for (int d=0; d<dim-1; ++d){
			for (int m=d+1; m<dim; ++m){
				Phi.row(count) = ((X.row(d).array() - X_mean(d))*(X.row(m).array() - X_mean(m))).matrix();
				gradPhi(d).row(count) = (X.row(m).array() - X_mean(m)).matrix();
				gradPhi(m).row(count) = (X.row(d).array() - X_mean(d)).matrix();
				count += 1;
			}
		}

		// Solve coefficients
		MatrixXd A = MatrixXd::Zero(L, L);
		VectorXd b = VectorXd::Zero(L);	
		VectorXd k = VectorXd::Zero(L);

		for (int l=0; l<L; l++){
			b(l) = h_diff.dot(Phi.row(l))/(static_cast<double>(N));
			for (int m=l; m<L; ++m){
				double sum = 0.0;
				for (int d=0; d<dim; ++d){
					sum += (gradPhi(d).row(l)).dot(gradPhi(d).row(m));
				}
				A(l,m) = sum/(static_cast<double>(N));
				A(m,l) = A(l,m);
			}
		}

		k = A.jacobiSvd(ComputeThinU | ComputeThinV).solve(b);

		for (int d=0; d<dim; ++d){
			K.row(d) = (k.transpose())*gradPhi(d);
		}

		for (int n=0; n<N; ++n){
			u.col(n) = K.col(n) + B;
		}

		return u;

	}

} // End of namespace optimization


