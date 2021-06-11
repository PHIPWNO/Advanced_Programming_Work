cmake_minimum_required(VERSION 3.13)
project(C_and_Cuda_Code C)

set(CMAKE_C_STANDARD 99)


# ======================================================================
# Subdirectories
# ======================================================================

add_subdirectory(sphere_ray_tracing_C)



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