#ifndef PRIOR
#define PRIOR

#include <iostream>
#include <random>

#include <Eigen/Dense>


// using namespace std;


namespace optimization{

	using namespace Eigen;

	class Prior
	{

	public:

		virtual MatrixXd sample_IC(int N, std::default_random_engine &generator) = 0; // Pure virtual function
		virtual void message() = 0;

	};


	// Multivariate Gaussian
	class Gaussian: public Prior
	{
		VectorXd mean;
		VectorXd std;

	public:
		Gaussian(){} // Default constructor for just declaring a Gaussian object

		Gaussian(const VectorXd &mean, const VectorXd &std)
			: mean(mean), std(std) {}

		explicit Gaussian(const int dim){
			mean = VectorXd::Constant(dim, 40);
			std = VectorXd::Constant(dim, 20);
		}

		virtual MatrixXd sample_IC(const int num_particles, std::default_random_engine &generator) override;

		virtual void message() override;

		// friend ostream& operator<<(ostream &out, const Gaussian) 

	};


	class Uniform: public Prior
	{
		VectorXd l;
		VectorXd r;


	public:
		Uniform(){} // Default constructor for just declaring the Uniform object

		Uniform(const VectorXd &l_range, const VectorXd &r_range)
			: l(l_range), r(r_range) {}

		explicit Uniform(const int dim){
			l = VectorXd::Constant(dim, -40);
			r = VectorXd::Constant(dim, 40);
		}

		virtual MatrixXd sample_IC(const int num_particles, std::default_random_engine &generator) override;

		virtual void message() override;

	};


} // End of namespace optimization

#endif


