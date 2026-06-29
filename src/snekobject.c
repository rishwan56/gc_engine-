#include "snekobject.h"
#include <string.h>
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

bool snek_array_set(snek_object_t* array, size_t index, snek_object_t* value){
	if(array== NULL || value == NULL) return false; 
	if(array -> kind != ARRAY) return false; 
	snek_array_t arr = array -> data.v_array; 
	if(index >= arr.size) return false; 
	arr.elements[index] = value; 
	return true; 
} 

snek_object_t *snek_array_get(snek_object_t *array, size_t index){
	if(array == NULL) return NULL; 
	if(array-> kind != ARRAY) return NULL; 
	snek_array_t arr = array -> data.v_array; 
        if(arr.size <= index) return false; 
	return 	arr.elements[index]; 
} 

int snek_len(snek_object_t *obj){
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
				int len = strlen(a -> data.v_string) + strlen(b -> data.v_string) - 1; 
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
				return new_snek_vector(snek_add(a -> data.v_vector.x, b -> data.v_vector.x), snek_add(a -> data.v_vector.y, b -> data.v_vector.y), 
						snek_add(a -> data.v_vector.z, b -> data.v_vector.z)); 
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
				return arr; 
		}
		default : return NULL ;
	}
}
