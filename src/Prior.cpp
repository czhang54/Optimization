#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include <Eigen/Dense>

// #include "Optimizer.h"
// #include "World.h"
#include "Prior.h"

using namespace std;
using namespace Eigen;


/* Gaussian definitions */

MatrixXd Gaussian::sample_IC(int N) {

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

void Gaussian::message() {
	cout << "Initial distribution is Gaussian" << '\n';
}



/* Uniform definitions */

MatrixXd Uniform::sample_IC(int N) {

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

void Uniform::message() {
	cout << "Initial distribution is Uniform" << '\n';
}


