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

	virtual MatrixXd sample_IC(int N) override {

		default_random_engine generator;
		// Seeding generator to get NEW random values every time compiled!
		generator.seed(static_cast<unsigned int>(std::time(0))); 

		MatrixXd particles(mean.size(), N);
		for (int d=0; d<mean.size(); d++){
			normal_distribution<double> normal(mean(d), std(d));
			RowVectorXd particles_d(N);
			for (int i=0; i<N; i++){
				particles_d(i) = normal(generator);
			}
			particles.row(d) = particles_d;
		}

		return particles;	
	}

	virtual void message() override {
		cout << "Initial distribution is Gaussian" << '\n';
	}

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

	virtual MatrixXd sample_IC(int N) override {

		default_random_engine generator;
		generator.seed(static_cast<unsigned int>(std::time(0))); 

		MatrixXd particles;
		for (int d=0; d<l.size(); d++){
			uniform_real_distribution<double> uniform(l(d), r(d));
			RowVectorXd particles_d(N);
			for (int i=0; i<N; i++){
				particles_d(i) = uniform(generator);
			}
			particles.row(d) = particles_d;
		}

		return particles;	
	}

	virtual void message() override {
		cout << "Initial distribution is Uniform" << '\n';
	}

};

#endif


