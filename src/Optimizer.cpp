#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "World.h"
#include "Optimizer.h"
#include "Prior.h"
#include "Controller.h"

using namespace std;

typedef Matrix<MatrixXd, Dynamic, 1> Tensor3Xd; // 3d array (tensor)


Optimizer::Optimizer(const World *w, const int num_particles, string optimizer_name){
	N = num_particles;
	name = optimizer_name;

	// May move this into initialize() ?
	Tensor3Xd Value(w->get_time());
	for (int i=0; i<w->get_time(); ++i){
		Value(i) = MatrixXd::Zero(w->get_dim(), N);
	}
	value = Value;
}

Tensor3Xd Optimizer::getValue() const {return value;}
string Optimizer::getName() const {return name;}
World* Optimizer::get_world() const {return world;}

void Optimizer::setPrior(Prior *p){prior = p;}

void Optimizer::initialize(){
	value[0] = prior->sample_IC(N);
}

void Optimizer::setController(Controller *c){
	controller = c;
	// cout << "set controller..." << '\n';
	c->optimizer = this; // Works now
}

void Optimizer::update(int TI, double dt){
	value(TI+1) = controller->f(value(TI), TI, dt);
}

ostream& operator<<(ostream &out, const Optimizer &optimizer){
	out << "You are using " << optimizer.name << " method" << '\n';
	return out;
}



