#include <iostream>
#include <cmath>
#include <random>

#include <Eigen/Dense>

#include "Obj_fn.h"

// using namespace std;
// using namespace Eigen;


namespace optimization{

	/* Obj_fn (base class) definitions */

	double Obj_fn::getMin() const {return h_min;}

	Eigen::VectorXd Obj_fn::global_minimizer() const {
		return Eigen::VectorXd::Zero(m_dim);
	}

	Eigen::RowVectorXd Obj_fn::evaluate(const Eigen::MatrixXd &X){
		Eigen::RowVectorXd h = Eigen::RowVectorXd::Constant(X.cols(), 0.0);
		return h;
	} 

	std::ostream& operator<<(std::ostream &out, const Obj_fn *fn){
		return fn->message(out);
	}

	std::ostream& Obj_fn::message(std::ostream &out) const {
		out << "You are optimizing a function" << '\n'; 
		return out;
	}


	/* Rastrigin function definitions */

	// double Rastrigin::getMin() const {return 0.0;}

	Eigen::VectorXd Rastrigin::global_minimizer() const {
		return Eigen::VectorXd::Zero(m_dim);
	}

	std::ostream& Rastrigin::message(std::ostream &out) const {
		out << "You are optimizing Rastrigin function" << '\n';
		return out;
	}

	/* Evaluate Rastrigin function */
	Eigen::RowVectorXd Rastrigin::evaluate(const Eigen::MatrixXd &X) {

		Eigen::RowVectorXd h = Eigen::RowVectorXd::Constant(X.cols(), 0.0);

		for (int i=0; i<X.cols(); ++i){
			double num = 0;
			for (int d=0; d<m_dim; ++d){
				num += std::pow(X(d,i), 2) - 10*std::cos(2*M_PI*X(d,i));
			}
			h(i) = 10*m_dim + num;
		}

		return h;
	}


	/* Quadratic function definitions */

	// double Quadratic::getMin() const {return 0.0;}

	Eigen::VectorXd Quadratic::global_minimizer() const {
		return Eigen::VectorXd::Zero(m_dim);
	}

	std::ostream& Quadratic::message(std::ostream &out) const {
		out << "You are optimizing Quadratic function" << '\n';
		return out;
	}

	/* Evaluate Quadratic function */
	Eigen::RowVectorXd Quadratic::evaluate(const Eigen::MatrixXd &X) {

		Eigen::RowVectorXd h = Eigen::RowVectorXd::Constant(X.cols(), 0.0);

		for (int i=0; i<X.cols(); ++i){
			Eigen::VectorXd v = X.col(i);
			h(i) = 0.5*v.transpose()*H_*v;
		}
		return h;
	}


} // End of namespace optimization
