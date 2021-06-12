cmake_minimum_required(VERSION 3.13)
project(C_and_Cuda_Code C)

set(CMAKE_C_STANDARD 99)


# ======================================================================
# Link to standard math library
# ======================================================================

include(CheckLibraryExists)
check_library_exists(m pow "" LIBM_FOUND)
if (LIBM_FOUND)
    link_libraries(m)
else ()
    message(FATAL_ERROR "Yikes, looks like you don't have the std math library!")
endif ()

# ======================================================================
# Find OpenMP and CUDA
# ======================================================================

find_package(OpenMP)

include(CheckLanguage)
check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
endif ()

# ======================================================================
# Common Libraries
# ======================================================================

# Bitmap Writers
# Credit: Ron Rahaman
add_library(bitmap helper_libraries/bitmap/bitmap.c)
target_include_directories(bitmap PUBLIC helper_libraries/bitmap/)


# CUDA helper functions
if (CMAKE_CUDA_COMPILER)
    #interface because the library is all h files
    add_library(helper_cuda INTERFACE)
    target_include_directories(helper_cuda INTERFACE helper_libraries/helper_cuda/)
    #links the bitmap library to it
    set_target_properties(bitmap PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    
endif ()


# ======================================================================
# Subdirectories
# ======================================================================

add_subdirectory(sphere_ray_tracing_C)
add_subdirectory(sphere_ray_tracing_cuda)

