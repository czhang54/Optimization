#ifndef CONTROLLER
#define CONTROLLER

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class Optimizer;
// class Poisson;


class Controller
{	

protected:
	// These are set via Optimizer::setController
	Optimizer *optimizer;
	int T;

	// Performace metric
	RowVectorXd h_hat;
	RowVectorXd h_best;
	RowVectorXd error;

public:

	Controller(int num_times) {
		T = num_times;
		h_hat = RowVectorXd::Zero(T);
		h_best = RowVectorXd::Zero(T);
	}

	virtual MatrixXd f(const MatrixXd &X, int TI, double dt);

	friend void Optimizer::setController(Controller *c); // Friending a single member function works here (Why?) 
	// friend class Optimizer; // Or we can friend the entire class

	friend ostream& operator<<(ostream &out, const Controller *c);

	vector<RowVectorXd> performance(){
		vector<RowVectorXd> p;
		p.push_back(h_hat);
		p.push_back(h_best);
		return p;
	}

};


class CPF: public Controller
{
	string solver = "Affine"; // Default is "Affine"
	// Poisson *poisson_solver;

public:

	CPF(int num_times, string solver_name="Affine")
		: Controller(num_times), solver(solver_name) {
			// if (solver.compare("Affine") == 0){
			// 	poisson_solver = &Affine()
			}

	virtual MatrixXd f(const MatrixXd &X, int TI, double dt);

	MatrixXd Affine(const MatrixXd &X, const RowVectorXd &h_diff);

};


























#endif