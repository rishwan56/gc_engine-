#include "sneknew.h"
#include <stdlib.h>
snek_object_t *_new_snek_object(){
	snek_object_t *obj = calloc(1, sizeof(snek_object_t)); 
	if(obj == NULL) return NULL;
	
	obj -> refcount = 1; 
	return obj;
}

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
	switch(obj -> kind){
		case(INTEGER) : break; 
		case(FLOAT) : break; 
		case(STRING) : {
			free(obj -> data.v_string);
			break; 
		}
		case(VECTOR3) : {
			refcount_dec(obj -> data.v_vector.x);
			refcount_dec(obj -> data.v_vector.y);
			refcount_dec(obj -> data.v_vector.z);
			break; 
		}
		case(ARRAY) : {
	 		snek_array_t arr = obj -> data.v_array; 
			for(int i = 0; i < arr.size; i++)refcount_dec(arr.elements[i]);
			break; 
		}
		free(obj); 				
	}
}

snek_object_t *new_snek_integer(int value){
	snek_object_t* obj = _new_snek_object(); 
	if(obj == NULL) return NULL;

	obj -> kind = INTEGER; 
	obj -> data.v_int = value; 
	return obj; 
}

snek_object_t *new_snek_float(float value){
	snek_object_t *obj = _new_snek_object();
	if(obj == NULL) return NULL;

	obj -> kind = FLOAT; 
	obj -> data.v_float = value; 
	return obj; 
}

snek_object_t *new_snek_string(char* string){
	size_t size = strlen(string);
	snek_object_t *obj = _new_snek_object(); 
	if(obj == NULL) return NULL; 

	obj -> kind = STRING;	
	obj -> data.v_string = malloc(size + 1); 
	strcpy(obj -> data.v_string, string); 
	return obj;
}

snek_object_t *new_snek_vector(snek_object_t *X, snek_object_t *Y, snek_object_t *Z){
	snek_object_t *obj = _new_snek_object(); 
	if(obj == NULL) return NULL;
	
	obj -> kind = VECTOR3;
	obj -> data.v_vector.x = X; 
	obj -> data.v_vector.y = Y; 
	obj -> data.v_vector.z = Z; 
       	return obj;	
}

snek_object_t *new_snek_array(size_t size){
	snek_object_t *obj = _new_snek_object(); 
	if(obj == NULL) return NULL;
	
	obj -> kind = ARRAY;
	obj -> data.v_array.size = size;
	
	obj -> data.v_array.elements = calloc(size, sizeof(snek_object_t*)); 

	return obj; 
}
