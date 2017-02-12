# Optimization_Cpp

This repository contains C++ codes for simulating global optimization algorithms. 
It is designed to be flexible for the user to add:
  + Objective functions
  + Initialization methods for the optimizers
  + Specific optimization algorithms

Directories:
  + include: contains all the headers
  + src: contains all the source files
  + example: contains the interface to start the simulation
 
Requirement:
  + Eigen: a convenient matrix library that can be downloaded at http://eigen.tuxfamily.org/index.php?title=Main_Page#Download
 
The project was tested on Mac OS 10.12 using g++ 4.2.1
To run in terminal: 
  cd example
  g++ -std=c++11 -I ./../include -c ./../src/*.cpp example.cpp
  g++ example.o Prior.o Obj_fn.o Algorithm.o -o example
  ./example
