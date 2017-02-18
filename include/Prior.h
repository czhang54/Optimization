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
		VectorXd mean_;
		VectorXd std_;

	public:

		/* Default constructor for just declaring a Gaussian object */
		Gaussian(){} 

		/* Constructor with input mean and standard deviation */
		Gaussian(const VectorXd &mean, const VectorXd &std)
			: mean_(mean), std_(std) {}

		/* Constructor with only dimension parameter. 
		Used if mean and standard deviation are not frequently changed. */
		explicit Gaussian(const int dim){
			mean_ = VectorXd::Constant(dim, 40);
			std_ = VectorXd::Constant(dim, 20);
		}

		/* Generate initial samples from Gaussian distribution */
		virtual MatrixXd sample_IC(const int num_particles, std::default_random_engine &generator) override;

		virtual void message() override;

		// friend ostream& operator<<(ostream &out, const Gaussian) 

	};


	class Uniform: public Prior
	{
		VectorXd l_range_;
		VectorXd r_range_;


	public:

		/* Default constructor for just declaring a Uniform object */
		Uniform(){} // Default constructor for just declaring the Uniform object

		/* Constructor with parameters specifying the range in each dimension */
		Uniform(const VectorXd &l_range, const VectorXd &r_range)
			: l_range_(l_range), r_range_(r_range) {}

		/* Constructor with only dimension parameter. 
		Used if range parameters are not frequently changed. */		
		explicit Uniform(const int dim){
			l_range_ = VectorXd::Constant(dim, -40);
			r_range_ = VectorXd::Constant(dim, 40);
		}

		/* Generate initial samples from Uniform distribution */
		virtual MatrixXd sample_IC(const int num_particles, std::default_random_engine &generator) override;

		virtual void message() override;

	};


} // End of namespace optimization

#endif


