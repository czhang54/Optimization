
add_library(optimization_source
	Prior.cpp 
	Obj_fn.cpp 
	Algorithms/Algorithm.cpp 
	Algorithms/SimulatedAnnealing.cpp 
	Algorithms/CrossEntropy.cpp 
	Algorithms/ParticleFilter.cpp 
	Algorithms/ControlledParticleFilter.cpp
)

# Include headers from a separate directory
include_directories(../include) 

