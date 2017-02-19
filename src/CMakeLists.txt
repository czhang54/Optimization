
add_library(optimization_source
	Prior.cpp 
	Obj_fn.cpp 
	Algorithm.cpp 
	CrossEntropy.cpp 
	ControlledParticleFilter.cpp
)

# Include headers from a separate directory
include_directories(../include) 

