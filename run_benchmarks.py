import os 
import subprocess 

def run_cmd(cmd, cwd = "."): 
    subprocess.run(cmd, shell = True, check = True, cwd = cwd)

def read_result(filename):
    if os.path.exists(filename) : 
        with open(filename, "r") as f: 
            data = f.read().split()
            return float(data[0]), int (data[1])
    return 0.0, 0

def main(): 
    build_dir = "build"
    if not os.path.exists(build_dir): 
        os.makedirs(build_dir)

    #1. Benchmark
    print("[1/2] Compiling Reference Counting...")
    run_cmd("cmake -DUSE_MARK_SWEEP=OFF ..", cwd = build_dir)
    run_cmd("make gc_benchmark", cwd = build_dir)
    print("Running Reference Counting Benchmark ...")
    run_cmd("./gc_benchmark", cwd = build_dir)


