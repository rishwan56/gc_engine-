#pragma once 

#include"snekobject.h" 
#include "vm.h"
extern int total_live_allocation; 

snek_object_t* new_snek_integer(vm_t* t, int value);
snek_object_t* new_snek_float(vm_t* t, float value);
snek_object_t* new_snek_string(vm_t*, char* value); 
snek_object_t* new_snek_vector3(vm_t* vm, snek_object_t* x,  snek_object_t* y, snek_object_t* z); 
snek_object_t* new_snek_array(vm_t* vm, size_t size); 

