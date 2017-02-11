#ifndef PRIOR
#define PRIOR

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include <Eigen/Dense>

// #include "Optimizer.h"
// #include "World.h"

using namespace std;
using namespace Eigen;


class Prior
{

public:

	virtual MatrixXd sample_IC(int N) = 0; // Pure virtual function
	virtual void message() = 0;

};


// Multivariate Gaussian
class Gaussian: public Prior
{
	VectorXd mean;
	VectorXd std;

public:
	// Generate Gaussian samples. Need more efficient method!
	Gaussian(const VectorXd &mean, const VectorXd &std)
		: mean(mean), std(std) {}

	virtual MatrixXd sample_IC(int N) override;

	virtual void message() override;

	// friend ostream& operator<<(ostream &out, const Gaussian) 

};


class Uniform: public Prior
{
	VectorXd l;
	VectorXd r;


public:
	// Generate Gaussian samples. Need more efficient method!
	Uniform(const VectorXd &l_range, const VectorXd &r_range)
		: l(l_range), r(r_range) {}

	virtual MatrixXd sample_IC(int N) override;

	virtual void message() override;

};



#endif


