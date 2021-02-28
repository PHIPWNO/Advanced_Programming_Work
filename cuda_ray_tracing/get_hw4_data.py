from subprocess import run, CalledProcessError
import os
import sys
from pathlib import Path
from pandas import DataFrame

#Gets current directory
CURRENT_DIR = Path(os.getcwd())
#Path to build directory
BUILD_DIR = CURRENT_DIR / 'build'

#Path to directory with executable
EXEC_DIR = BUILD_DIR

#name of CPU executable
CPU_EXEC = 'CpuRayTrace'

#name of gpu executable
GPU_EXEC = 'CudaRayTrace'
BLOCK_SIZE = 8
GRID_SIZE = 2048

# Default number of timesteps and particles
N = 1024
MAX_CPUS = os.cpu_count()

def run_cmake(DC_MAKE_VARS=['-DCMAKE_CUDA_COMPILER=nvcc', '-DCMAKE_BUILD_TYPE=Release'], display_output=False):
    '''
    Has Cmake run its stuff in the build directory.
    DC_MAKE_VARS is a list of variables to also pass
    display_output if true will print the result of the cmake
    '''
    # changes bytes into string y.stdout.decode("utf-8")
    ARG_LIST = ['cmake'] + DC_MAKE_VARS + ['..']
    try:
        output = run(ARG_LIST, cwd = BUILD_DIR, capture_output = True, check=True)
        if display_output:
            print(output.stdout.decode("utf-8"))
        return True
    except CalledProcessError as error:
        print("ERROR!\nSOMETHING WENT WRONG WITH CMAKE")
        print(error.stderr.decode('utf-8'))
        return False


def build_executable(makefile_dir):
    '''
    Runs the makefile for a given directory
    '''
    try:
        output = run(['make'], cwd = makefile_dir, capture_output = True, check=True)
        print(output.stdout.decode("utf-8"))
        return True
    except CalledProcessError as error:
        print(f"ERROR!\nSOMETHING WENT WRONG WITH THE MAKEFILE IN {makefile_dir}")
        print(error.stderr.decode('utf-8'))
        return False


def get_variable_grid_data(n, thread_range):
    '''
    Gathers time for a range of grid sizes for a given optimal block size
    '''
    #unpacks thread_range
    start, end, step = thread_range
    block_size = str(BLOCK_SIZE)
    print(f"Gathering Grid Size Data for n = {n} with block size {block_size}")

    timings = {2**i:0 for i in range(start, end, step)}

    #runs for each value in thread range
    for i in range(start, end, step):
        grid_size = 2 ** i
        run_args = [f'./{GPU_EXEC}', str(n), str(grid_size), 
            str(grid_size), block_size, block_size]
        #Stops program if invalid input is passed
        try:
            output = run(run_args, cwd = EXEC_DIR, capture_output = True, check=True)
        except CalledProcessError as error:
            print(f"ERROR!\nSOMETHING WENT WRONG WITH THE FOLLOWING COMMAND "
            + "LINE INPUT\n", run_args)
            print(error.stderr.decode('utf-8'))
            return -1
        
        timings[grid_size] = float(output.stdout.decode('utf-8'))

        print(f"Grid size {grid_size} x {grid_size}: {timings[grid_size]} ms")


    print("\n")
    header_row = ["Grid size", "Time(microseconds)", "Dimension"]
    output = [[2**i, timings[2**i],  n] for i in range(start, end, step)]
    output.insert(0, header_row)
    output = DataFrame(output).to_string(header = False, index = False)

    with open("grid_data.txt", "w") as file:
        file.write("Grid Size Data\n")
        file.write(f"Image Dimensions:{n}x{n}\n")
        file.write(f"Block Dimensions:{block_size}x{block_size}\n")
        file.write(f"Grid range:{start} to {end}\n\n")
        file.write(output)
        file.write('\n')
        
    return 0


def get_variable_size_data(dim_range):
    '''
    Tests OpenMP and GPU implementation on various dimension sizes and fixed
    hardware parameters
    '''
    ThreadCount = MAX_CPUS
    block_size = str(BLOCK_SIZE)
    grid_size = str(GRID_SIZE)
    start, end, step = dim_range

    print(f"Gathering variable dimension times with grid dim of {grid_size}"
    + f" and block size {block_size}. Also cpu thread count {ThreadCount}")

    #First tuple is cpu time, second tuple is gpu time
    timings = {i:[0,0] for i in range(start, end, step)}

    for dim in range(start, end, step):
        cpu_run_args = [f'./{CPU_EXEC}', str(dim)]
        cpu_output = run(cpu_run_args, cwd = EXEC_DIR, capture_output = True, 
        check=True, env=({'OMP_NUM_THREADS':str(ThreadCount)}))
        timings[dim][0] = float(cpu_output.stdout.decode('utf-8')) * 1000000

        gpu_run_args = [f'./{GPU_EXEC}', str(dim), grid_size, grid_size,
        block_size, block_size]
        gpu_output = run(gpu_run_args, cwd = EXEC_DIR, capture_output = True, 
        check=True)
        timings[dim][1] = float(gpu_output.stdout.decode('utf-8'))

        print(f"{dim} x {dim}: CPU Time:{timings[dim][0]} micro seconds\t"
        + f"GPU Time:{timings[dim][1]}")

    print("\n")
    header_row = ["Dimension", "CPU Time(micro seconds)", "GPU Time(micro seconds)"]
    output = [[i, timings[i][0], timings[i][1]] 
        for i in range(start, end, step)]

    output.insert(0, header_row)
    output = DataFrame(output).to_string(header = False, index = False)

    with open("variable_dimension_time.txt", "w") as file:
        file.write("Variable Dimension Timings\n")
        file.write(f"Grid Dimensions:{grid_size}x{grid_size}\n")
        file.write(f"Block Dimensions:{block_size}x{block_size}\n")
        file.write(f"CPU Threads:{ThreadCount}\n\n")
        file.write(output)
        file.write("\n")
        
    return 0


if __name__ == "__main__":
    if not os.path.exists('build'):
        os.makedirs('build')
    if not run_cmake():
        print("ERROR RUNNING CMAKE!\nPROCESS STOPPED")
        sys.exit(1)
    if not build_executable(EXEC_DIR):
        print("ERROR RUNNING WITH COMPILING WITH THE MAKEFILE!\
        \nPROCESS STOPPED")
        sys.exit(1)
    n_args = len(sys.argv)
    n = N
    max_cpus = MAX_CPUS

    #When three args just tests the variable grid size stuff
    if(n_args == 3):
        n = int(sys.argv[1])
        thread_range = sys.argv[2].replace('[', ' ').replace(']', ' ')
        thread_range = thread_range.replace(',', ' ').split()
        thread_range = [int(i) for i in thread_range]
        thread_range[1] += 1

        print(f"Testing with n = {n} grid sizes from {thread_range[0]}" +
        f"to {thread_range[1]} with step size {thread_range[2]}")

        if get_variable_grid_data(n, thread_range):
            sys.exit(-1)
    #When 4 args it also tests variable time 
    elif(n_args == 2):
        dim_range = sys.argv[1].replace('[', ' ').replace(']', ' ')
        dim_range = dim_range.replace(',', ' ').split()
        dim_range = [int(i) for i in dim_range]
        dim_range[1] += 1

        print(f"Testing with dimmensions from {dim_range[0]}" +
        f"to {dim_range[1]} with step size {dim_range[2]}")

        get_variable_size_data(dim_range)

    else:
        print("NOT ENOUGH COMMAND LINE ARGS PASSED IN!\n"
            +"for just variable grid testing pass:\n"
            + "img_size, [start,end,step]\n"
            +"or for variable image size testing:\n"
            +"[start,end,step]")
        sys.exit(1)

    sys.exit(0)

