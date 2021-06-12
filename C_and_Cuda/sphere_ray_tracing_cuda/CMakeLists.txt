#Cmake file for generating a sphere using ray tracing in cuda

# Ensures it only runs if there's a cuda compiler
if (CMAKE_CUDA_COMPILER)
	#creates executable
	add_executable(CudaRayTrace main/ray_trace_gpu.cu)
	set_target_properties(CudaRayTrace PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
	#lists the libraries to link to compiling this executable
	target_link_libraries(CudaRayTrace PUBLIC helper_cuda bitmap)
	#link math library
	link_libraries(m)
	#Just shows resource info upon compilation
	target_compile_options(CudaRayTrace PUBLIC --resource-usage)

endif ()
