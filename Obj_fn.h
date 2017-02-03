#ifndef OBJ_FNS
#define OBJ_FNS

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class Obj_fn
{
private:
	string m_name; 
	double h_min;

protected:
	int m_dim;

public:
	Obj_fn(string name, int dim, double h_min)
		: m_name(name), m_dim(dim), h_min(h_min) {}

	virtual double getMin() const {return 0.0;}

	virtual RowVectorXd evaluate(MatrixXd X){
		RowVectorXd h = RowVectorXd::Constant(X.cols(), 0.0);
		return h;
	} // Derived classes are forced to define evaluate function

	friend ostream& operator<<(ostream &out, const Obj_fn *fn){
		return fn->message(out);
	}

	virtual ostream& message(ostream &out) const {
		out << "You are optimizing a function" << '\n'; 
		return out;
	}

};


class Rastrigin: public Obj_fn
{

public:

	Rastrigin(int dim): Obj_fn("Rastrigin", dim, 0.0) {}

	virtual double getMin() const override {return 0.0;}

	virtual ostream& message(ostream &out) const override {
		out << "You are optimizing Rastrigin function" << '\n';
		return out;
	}

	// Virtual functions MUST BE DEFINED INSIDE THE CLASS? (NOT NECESSARILY)
	virtual RowVectorXd evaluate(MatrixXd X) override {

		RowVectorXd h = RowVectorXd::Constant(X.cols(), 0.0);

		for (int i=0; i<X.cols(); ++i){
			double num = 0;
			for (int d=0; d<m_dim; ++d){
				num += pow(X(d,i), 2) - 10*cos(2*acos(-1)*X(d,i));
			}
			h(i) = 10*m_dim + num;
		}

		return h;
	}
};


class Quadratic: public Obj_fn
{
	MatrixXd H;

public:

	Quadratic(int dim): Obj_fn("Quadratic", dim, 0.0) {
		H = MatrixXd::Identity(m_dim, m_dim);
	}

	virtual double getMin() const override {return 0.0;}

	virtual ostream& message(ostream &out) const override {
		out << "You are optimizing Quadratic function" << '\n';
		return out;
	}

	virtual RowVectorXd evaluate(MatrixXd X) override {

		RowVectorXd h = RowVectorXd::Constant(X.cols(), 0.0);

		for (int i=0; i<X.cols(); ++i){
			VectorXd v = X.col(i);
			h(i) = 0.5*v.transpose()*H*v;
		}
		return h;
	}

};








#endif