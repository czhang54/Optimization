#ifndef OBJ_FN
#define OBJ_FN

#include <iostream>
#include <string>

#include <Eigen/Dense>

// using namespace std;
// using namespace Eigen;

namespace optimization{

	/* Base class for all obejctive functions */

	class Obj_fn
	{
	protected:
		std::string m_name; 
		int m_dim;
		double h_min;

	public:
		Obj_fn(std::string name, int dim, double h_min)
			: m_name(name), m_dim(dim), h_min(h_min) {}

		virtual double getMin() const;

		virtual Eigen::VectorXd global_minimizer() const;

		virtual Eigen::RowVectorXd evaluate(const Eigen::MatrixXd &X);

		friend std::ostream& operator<<(std::ostream &out, const Obj_fn *fn);

		virtual std::ostream& message(std::ostream &out) const;

	};


	/* Rastrigin function */

	class Rastrigin: public Obj_fn
	{

	public:

		Rastrigin(int dim): Obj_fn("Rastrigin", dim, 0.0) {}

		virtual double getMin() const override;

		virtual Eigen::VectorXd global_minimizer() const override;

		virtual std::ostream& message(std::ostream &out) const override;

		virtual Eigen::RowVectorXd evaluate(const Eigen::MatrixXd &X) override;
	};


	/* Quadratic function */

	class Quadratic: public Obj_fn
	{
		Eigen::MatrixXd H;

	public:

		Quadratic(int dim): Obj_fn("Quadratic", dim, 0.0) {
			H = Eigen::MatrixXd::Identity(m_dim, m_dim);
		}

		virtual double getMin() const override;

		virtual Eigen::VectorXd global_minimizer() const override;

		virtual std::ostream& message(std::ostream &out) const override ;

		virtual Eigen::RowVectorXd evaluate(const Eigen::MatrixXd &X) override;

	};


} // End of namespace optimization



#endif
