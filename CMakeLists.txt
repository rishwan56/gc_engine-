cmake_minimum_required(VERSION 3.10)

project(GarbageCollector C)

add_executable(
	gc_engine
	main.c 
	objects/snekobject.c
)

target_include_directories(gc_engine PRIVATE objects)


