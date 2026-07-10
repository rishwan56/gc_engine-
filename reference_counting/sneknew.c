#include "sneknew.h"
#include "refcount.h"
#include <stdlib.h>
#include <string.h>

//int total_live_allocation; 

snek_object_t *_new_snek_object(){
	snek_object_t *obj = calloc(1, sizeof(snek_object_t)); 
	if(obj == NULL) return NULL;
	total_live_allocation++; 
	obj -> refcount = 1; 
	return obj;
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
	obj -> data.v_vector3.x = X; 
	obj -> data.v_vector3.y = Y; 
	obj -> data.v_vector3.z = Z; 
	
	refcount_inc(X);
	refcount_inc(Y);
	refcount_inc(Z);
	
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
