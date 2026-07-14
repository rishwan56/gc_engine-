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

    #1. Benchmark Reference Counting
    print("[1/2] Compiling Reference Counting...")
    run_cmd("cmake -DUSE_MARK_SWEEP=OFF ..", cwd = build_dir)
    run_cmd("make gc_benchmark", cwd = build_dir)
    print("Running Reference Counting Benchmark ...")
    run_cmd("./gc_benchmark", cwd = build_dir)

    #2. Benchmark mark & Sweep 
    print("\n[2/2] Compiling Mark & Sweep...") 
    run_cmd("cmake -DUSE_MARK_SWEEP=ON ..", cwd = build_dir)
    run_cmd("make gc_benchmark", cwd = build_dir)
    print("Running Mark & Sweep Benchmark...")
    run_cmd("./gc_benchmark", cwd = build_dir)

    #3. Read & Compare Results
    ref_time, ref_mem = read_result(os.path.join(build_dir, "refcount_bench.txt"))
    ms_time, ms_mem = read_result(os.path.join(build_dir, "marksweep_bench.txt"))

    print("\n" + "="*50)
    print("             COMPARATVIE ANALAYSIS REPORT")
    print("="*50)
    print(f"{'Metric' : <25} | {'Reference Counting' : <20} | {'Mark & Sweep' : <15}")
    print("-"*50)
    print(f"{'Execution Time (sec)':<25} | {ref_time:<20.4f} | {ms_time:<15.4f}")   
    print(f"{'Peak Heap Objects':<25} | {ref_mem:<20} | {ms_mem:<15}")
    print("="*50)
    

    if ref_time > 0 and ms_time > 0:
        ratio = ref_time / ms_time if ref_time > ms_time else ms_time / ref_time    
        winner = "Mark & Sweep" if ref_time > ms_time else "Reference Counting"     
        print(f"\nWinner: {winner} is {ratio:.2f}x faster on this workload!")       
    print("="*50)
  
if __name__ == "__main__":
    main()

