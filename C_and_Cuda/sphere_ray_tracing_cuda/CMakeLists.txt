#Cmake file for generating a sphere using ray tracing in cuda

# Ensures it only runs if there's a cuda compiler
if (CMAKE_CUDA_COMPILER)
	#Just shows resource info upon compilation
	add_compile_options(--resource-usage)
	#creates executable
	add_executable(CudaRayTrace main/ray_trace_gpu.cu)
	#lists the libraries to link to compiling this executable
	target_link_libraries(CudaRayTrace PUBLIC helper_cuda bitmap)

endif ()
