
#Cmake file for generating a sphere using ray tracing in C
link_libraries(m)
#add the vector library
add_library(vector vector/vectors.c)
target_include_directories(vector PUBLIC vector/)


#Executable for testing the vector library
add_executable(vector_test main/vector_test.c)
target_link_libraries(vector_test PUBLIC vector)


#Executable for generating the sphere with ray tracing
add_executable(ray_tracer main/ray_tracing.c)
target_link_libraries(ray_tracer PUBLIC bitmap)
target_link_libraries(ray_tracer PUBLIC vector)

#link to ray tracer 
find_package(OpenMP)
if (OPENMP_FOUND)
    target_link_libraries(ray_tracer PRIVATE OpenMP::OpenMP_C)

endif()

