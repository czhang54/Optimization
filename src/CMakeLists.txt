
add_library(optimization_source
	Prior.cpp 
	Obj_fn.cpp 
	Algorithms/Algorithm.cpp 
	Algorithms/CrossEntropy.cpp 
	Algorithms/ControlledParticleFilter.cpp
)

# Include headers from a separate directory
include_directories(../include) 

