
#include "refcount.h" 
#include <stdlib.h>
#include <stdio.h>

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
	printf("[GC] Freeing object of kind %d at %p\n", obj->kind, (void*)obj);
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
}

