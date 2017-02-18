/*
Implementation of the Controlled Particle Filter (CPF) algorithm. 
*/

#include <iostream>
#include <cmath>

#include <Eigen/Dense>

#include "Obj_fn.h" 
#include "Algorithm.h" 
#include "ControlledParticleFilter.h"


namespace optimization{

	/* Execute Controlled Particle Filter (CPF) algorithm for one iteration */
	MatrixXd ControlledParticleFilter::run(const MatrixXd &X, int TI, double dt, std::default_random_engine &generator) {

		if (TI % 1 == 0)
			{std::cout << "TI = " << TI << '\n';}

		RowVectorXd h = obj_fn->evaluate(X); // Evaluate objective function at particles

		// Evaluate performance of current iteration
		performance(X, h, TI);
		
		RowVectorXd h_diff = h.array() - h_mean(TI);

		// Main part: calculate control function for each particle
		MatrixXd u = MatrixXd::Zero(X.rows(), X.cols());
		u = affine_control(X, -h_diff, u);

		return X + u*dt;
	}


	// Compute the affine control law
	MatrixXd& ControlledParticleFilter::affine_control(const MatrixXd &X, const RowVectorXd &h_diff, MatrixXd &u){

		typedef Matrix<MatrixXd, Dynamic, 1> Tensor3Xd; // 3d array (tensor)

		// int dim = X.rows();
		int N   = num_particles;
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



