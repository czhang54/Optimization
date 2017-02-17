#include <iostream>
#include <random>

#include <Eigen/Dense>

#include "Prior.h"

// using namespace std;


namespace optimization{

	using namespace Eigen;

	/* Gaussian definitions */

	MatrixXd Gaussian::sample_IC(const int num_particles, std::default_random_engine &generator) {

		std::cout << "Generating Gaussian samples..." << std::endl;

		MatrixXd particles(mean_.size(), num_particles);
		for (int d=0; d<mean_.size(); d++){
			std::normal_distribution<double> normal(mean_(d), std_(d));
			RowVectorXd particles_d(num_particles);
			for (int i=0; i<num_particles; i++){
				particles_d(i) = normal(generator);
			}
			particles.row(d) = particles_d;
		}

		return particles;	
	}

	void Gaussian::message() {
		std::cout << "Initial distribution is Gaussian" << '\n';
	}



	/* Uniform definitions */

	MatrixXd Uniform::sample_IC(const int num_particles, std::default_random_engine &generator) {

		std::cout << "Generating Uniform samples..." << std::endl;

		MatrixXd particles = MatrixXd::Zero(l_range_.size(), num_particles); // Needs to prescribe the size of particles
		for (int d=0; d<l_range_.size(); d++){
			std::uniform_real_distribution<double> uniform(l_range_(d), r_range_(d));
			RowVectorXd particles_d(num_particles);
			for (int i=0; i<num_particles; i++){
				particles_d(i) = uniform(generator);
			}
			particles.row(d) = particles_d;
		}

		return particles;	
	}

	void Uniform::message() {
		std::cout << "Initial distribution is Uniform" << '\n';
	}


} // End of namespace optimization



