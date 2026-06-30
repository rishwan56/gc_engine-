#pragma once 
#include "snekobject.h"


snek_object_t *new_snek_integer(int value); 
snek_object_t *new_snek_float(float value);
snek_object_t *new_snek_string(char* value); 
snek_object_t *new_snek_vector(snek_object_t *X, snek_object_t *Y, snek_object_t *Z); 
snek_object_t *new_snek_array(size_t size);

snek_object_t *_new_snek_object(); 


