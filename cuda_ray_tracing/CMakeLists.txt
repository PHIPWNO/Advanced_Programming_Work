cmake_minimum_required(VERSION 3.16)

project(mpcs-hpc-homework-4 C CUDA)

#links the m library for all future targets
link_libraries(m)

#creates a library target of c files
add_library(bitmap bitmap/bitmap.c)
#includes where to look for header files associated with that library
target_include_directories(bitmap PUBLIC bitmap/)

#if (CMAKE_CUDA_COMPILER)
# This means this would only happen if there was a CUDA compiler 

#endif ()

add_library(helper_cuda INTERFACE)
target_include_directories(helper_cuda INTERFACE helper_cuda/)

#ensures that bitmap compiles even tho its C code
set_target_properties(bitmap PROPERTIES CUDA_SEPARABLE_COMPILATION ON)


add_executable(CpuRayTrace ray_tracing/ray_tracing.c ray_tracing/vectors.c)
target_link_libraries(CpuRayTrace PUBLIC bitmap)


find_package(OpenMP)
if (OPENMP_FOUND)
	target_link_libraries(CpuRayTrace PUBLIC OpenMP::OpenMP_C)
endif()

add_compile_options(--resource-usage)
#creates executable
add_executable(CudaRayTrace ray_tracing/ray_trace_gpu.cu)
#lists the libraries to link to compiling this executable
target_link_libraries(CudaRayTrace PUBLIC helper_cuda bitmap)



