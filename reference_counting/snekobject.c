#include "snekobject.h"
#include "refcount.h"
#include <string.h>
#include <stdlib.h>

bool snek_array_set(snek_object_t* array, size_t index, snek_object_t* value){
	if(array== NULL || value == NULL) return false; 
	if(array -> kind != ARRAY) return false; 
	snek_array_t arr = array -> data.v_array; 
	if(index >= arr.size) return false; 
	
	refcount_inc(value);
	snek_object_t *old = arr.elements[index];
	arr.elements[index] = value; 
	refcount_dec(old);
	
	return true; 
} 

snek_object_t *snek_array_get(snek_object_t *array, size_t index){
	if(array == NULL) return NULL; 
	if(array-> kind != ARRAY) return NULL; 
	snek_array_t arr = array -> data.v_array; 
	if(arr.size <= index) return NULL; 
	return 	arr.elements[index]; 
}  

int snek_len(snek_object_t *obj){
	if (obj == NULL) return 0;
	switch(obj -> kind) {
		case(INTEGER) : return 1; 
		case(FLOAT) : return 1; 
		case(STRING) : {
				       int len = strlen(obj -> data.v_string); 
				       return len; 
			       } 
		case(VECTOR3): return 3; 
		case(ARRAY) : return obj -> data.v_array.size; 	       
	}
	return 0;
}

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b){
	switch(a -> kind){
		case(INTEGER):{
				switch(b -> kind){
					case INTEGER : return new_snek_integer(a -> data.v_int + b -> data.v_int);
					case FLOAT :  return new_snek_float(a -> data.v_int + b -> data.v_float); 	       
					default : return NULL; 
				}
		}
		case(FLOAT) : {
				switch(b -> kind){
					case INTEGER : return new_snek_float(a -> data.v_float + b -> data.v_int); 
					case FLOAT : return new_snek_float(a -> data.v_float + b -> data.v_float); 
					default : return NULL;
				}
		}
		case(STRING) : {
				if(b -> kind != STRING) return NULL; 
				int len = strlen(a -> data.v_string) + strlen(b -> data.v_string) + 1; 
				char* value = malloc(len); 
				if(value == NULL) return NULL ; 
				strcpy(value, a -> data.v_string); 
				strcat(value, b -> data.v_string);
			        snek_object_t *c = new_snek_string(value);
				free(value); 
				return c; 	
		}
		case(VECTOR3) : {
				if(b -> kind != VECTOR3) return NULL; 
				snek_object_t *new_x = snek_add(a -> data.v_vector3.x, b -> data.v_vector3.x);
				snek_object_t *new_y = snek_add(a -> data.v_vector3.y, b -> data.v_vector3.y);
				snek_object_t *new_z = snek_add(a -> data.v_vector3.z, b -> data.v_vector3.z);
				snek_object_t *res = new_snek_vector(new_x, new_y, new_z);
				refcount_dec(new_x);
				refcount_dec(new_y);
				refcount_dec(new_z);
				return res;
		} 
		case(ARRAY) : {
      				if(b -> kind != ARRAY) return NULL; 
				size_t len = a -> data.v_array.size + b -> data.v_array.size; 
				snek_object_t *arr = new_snek_array(len);
			        snek_array_t aarray = a -> data.v_array;
				snek_array_t barray = b -> data.v_array; 	
				snek_array_t carray = arr -> data.v_array; 
				memcpy(carray.elements, aarray.elements, aarray.size * sizeof(snek_object_t *));
				memcpy(((char *)carray.elements) + aarray.size * sizeof(snek_object_t *), barray.elements, barray.size * sizeof(snek_object_t *)); 
				for (size_t i = 0; i < len; i++) {
					refcount_inc(carray.elements[i]);
				}
				return arr; 
		}
		default : return NULL ;
	}
}

void snek_object_free(snek_object_t *obj) {
	if (obj == NULL) return;
	switch (obj->kind) {
		case INTEGER:
		case FLOAT:
			break;
		case STRING:
			free(obj->data.v_string);
			break;
		case VECTOR3:
			break;
		case ARRAY:
			free(obj->data.v_array.elements);
			break;
	}
	free(obj);
}
