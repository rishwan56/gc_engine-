cmake_minimum_required(VERSION 3.10)
project(GarbageCollector C)

option(USE_MARK_SWEEP "Build with Mark and Sweep GC" OFF)
if(USE_MARK_SWEEP) 
	add_executable(
		gc_engine
		main2.c 
		mark_and_sweep/snekobject.c 
		stack/stack.c
		mark_and_sweep/sneknew.c 
		mark_and_sweep/vm.c 
	)
	target_include_directories(gc_engine PRIVATE objects stack mark_and_sweep)
	target_compile_definitions(gc_engine PRIVATE USE_MARK_SWEEP)
	
	#ADD BENCHMARK TARGET 
	add_executable(
		gc_benchmark
		benchmark.c
		mark_and_sweep/snekobject.c 
		stack/stack.c 
		mark_and_sweep/sneknew.c 
		mark_and_sweep/vm.c 
	)
	target_include_directories(gc_benchmark PRIVATE objects stack mark_and_sweep)
	target_compile_definitions(gc_benchmark PRIVATE USE_MARK_SWEEP)
else()
	add_executable(
		gc_engine
		main.c	 
		reference_counting/snekobject.c
		reference_counting/sneknew.c 
		reference_counting/refcount.c
	)
	target_include_directories(gc_engine PRIVATE objects reference_counting stack)
	
	#ADD BENCHMARK TARGET 
	add_executable(
		gc_benchmark
		benchmark.c 
		reference_counting/snekobject.c
		reference_counting/sneknew.c 
		reference_counting/refcount.c 
	)	
	target_include_directories(gc_benchmark PRIVATE objects reference_counting stack)
endif()
	


