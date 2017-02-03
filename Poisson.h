#ifndef POISSON
#define POISSON

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;



class Affine
{
	int d;
	int N;
	int L;

public:
	Affine(int d, int N){
		this->d = d;
		this->N = N;
		L = d*(d+1)/2;
	}



};














#endif