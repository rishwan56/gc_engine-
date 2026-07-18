#pragma once
#include "sneknew.h"
void refcount_inc(snek_object_t* obj); 
void refcount_dec(snek_object_t* obj); 
void refcount_free(snek_object_t* obj); 
void rc_visualize_heap(void);                                                                                                                                                   
