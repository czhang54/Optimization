#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <Eigen/Dense>

#include "World.h"
#include "Prior.h"
#include "Optimizer.h"
#include "Controller.h" 
// #include "Poisson.h"

using namespace std;
using namespace Eigen;


// Controller::Controller(Optimizer *o){
	// optimizer = o;
	// T = o->getValue().size();
	// d = o->getValue()[0].size();
	// N = o->getValue()[0][0].size();
	// h_hat.resize(T);
// }

typedef Matrix<MatrixXd, Dynamic, 1> Tensor3Xd; // 3d array (tensor)


// DO NOT put 'virtual' keyword again!!
MatrixXd Controller::f(const MatrixXd &X, int TI, double dt){

	return X;
}

// Optional
ostream& operator<<(ostream &out, const Controller *c){
	out << "I am attached with " << c->optimizer->getName() << '\n';
	return out;
}

MatrixXd CPF::ff(const MatrixXd &X, int TI, double dt) {

	cout << "TI = " << TI << '\n';

	return X;
}


MatrixXd CPF::f(const MatrixXd &X, int TI, double dt) {

	cout << "TI = " << TI << '\n';

	MatrixXd C = X; // Make a copy of input X

	// Evaluate objective function
	RowVectorXd h = optimizer->get_world()->get_obj()->evaluate(C);
	// cout << h << '\n';
	// Calculate performance
	double hm = h.mean();
	h_hat(TI) = hm;
	cout << "h_hat = " << hm << '\n';

	double hb = h.minCoeff();
	if (TI == 0){h_best(TI) = hb;}
	else if (hb < h_best(TI-1)){h_best(TI) = hb;}
	else {h_best(TI) = h_best(TI-1);}
	cout << "h_best = " << h_best(TI) << '\n';
	

	RowVectorXd h_diff = h.array() - hm;

	MatrixXd u;

	if (solver.compare("Affine") == 0){
		u = Affine(C, -h_diff);
	}

	if (TI == T-2){
		double hm_final = (optimizer->get_world()->get_obj()->evaluate(C + u*dt)).mean();
		h_hat(TI+1) = hm_final;
	}

	return C + u*dt;
}


// Affine control law
MatrixXd CPF::Affine(const MatrixXd &X, const RowVectorXd &h_diff){

	int dim = X.rows();
	int N   = X.cols();
	int L   = dim*(dim+1)/2;

	MatrixXd  u = MatrixXd::Zero(dim, N);
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




