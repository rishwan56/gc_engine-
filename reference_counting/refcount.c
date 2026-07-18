
#include "refcount.h" 
#include <stdlib.h>
#include <stdio.h>
#define MAX_TRACKED_OBJECTS 1024                                                                                                                                                 
static snek_object_t *tracked_objects[MAX_TRACKED_OBJECTS];                                                                                                                      
static size_t tracked_count = 0;                                                                                                                                                 
  
// Call this in sneknew.c when a new object is allocated
void rc_track_object(snek_object_t *obj) {
	if (tracked_count < MAX_TRACKED_OBJECTS) {
            tracked_objects[tracked_count++] = obj;
        }
    }
  
// Call this in refcount_free when an object is deallocated
void rc_untrack_object(snek_object_t *obj) {
	for (size_t i = 0; i < tracked_count; i++) {
            if (tracked_objects[i] == obj) {
                tracked_objects[i] = tracked_objects[tracked_count - 1];
                tracked_count--;
                break;
            }
	}
}

int total_live_allocation; 

void refcount_inc(snek_object_t *obj){
	if(obj == NULL) return;
	obj -> refcount ++; 

	return; 
}

void refcount_dec(snek_object_t *obj){
	if(obj == NULL) return ; 
	obj -> refcount --; 
	if(obj -> refcount <= 0) refcount_free(obj); 
	return ; 
}

void refcount_free(snek_object_t *obj){
	//printf("[GC] Freeing object of kind %d at %p\n", obj->kind, (void*)obj);
	switch(obj -> kind){
		case(INTEGER) : break; 
		case(FLOAT) : break; 
		case(STRING) : {
			free(obj -> data.v_string);
			break; 
		}
		case(VECTOR3) : {
			refcount_dec(obj -> data.v_vector3.x);
			refcount_dec(obj -> data.v_vector3.y);
			refcount_dec(obj -> data.v_vector3.z);
			break; 
		}
		case(ARRAY) : {
	 		snek_array_t arr = obj -> data.v_array; 
			for(size_t i = 0; i < arr.size; i++)refcount_dec(arr.elements[i]);
			free(arr.elements);
			break; 
		}
	}
	free(obj); 				
	total_live_allocation--; 
}

