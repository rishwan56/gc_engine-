#include <stdio.h>
#include "snekobject.h"
#include "sneknew.h"

int main(){
	//integer check 
	snek_object_t* inta = new_snek_integer(5); 
	printf("int:\nkind = %d, data = %d\n\n", inta -> kind, inta -> data.v_int); 
	
	//float check
	snek_object_t* floata = new_snek_float(3.14);
	printf("float:\nkind = %d, data = %f\n\n", floata -> kind, floata -> data.v_float);
	
	//string check
	snek_object_t *stringa = new_snek_string("rishwan"); 
	printf("string:\nkind = %d, data = %s\n\n", stringa -> kind, stringa -> data.v_string); 
	
	//vector 3 check 
	snek_object_t *vectora = new_snek_vector(inta, floata, stringa); 
	printf("vector3:\nkind = %d\ndata:\n", vectora -> kind);
	printf("x = %d\ny = %f\nz = %s\n\n", vectora -> data.v_vector.x -> data.v_int,  vectora -> data.v_vector.y -> data.v_float, vectora -> data.v_vector.z -> data.v_string); 

	//array check 
	snek_object_t *arr = new_snek_array(4); 
	snek_array_set(arr, 0, inta); 
	snek_object_t *ae1 = snek_array_get(arr, 0);
       	snek_array_set(arr, 1, floata); 
	snek_array_set(arr, 2, stringa); 
	snek_array_set(arr, 3, vectora); 
	printf("array:\nkind = %d\ndata:\n", arr -> kind); 
	snek_array_t array = arr -> data.v_array; 
	printf("%d\n", array.elements[0] -> data.v_int); 
	printf("%f\n", array.elements[1] -> data.v_float); 
	printf("%s\n", array.elements[2] -> data.v_string); 
	snek_object_t *vector = array.elements[3]; 
	printf("x = %d\ny = %f\nz = %s\n\n", vector -> data.v_vector.x -> data.v_int,  vector -> data.v_vector.y -> data.v_float, vector -> data.v_vector.z -> data.v_string); 
		
	//len :
	printf("length:\nint = %d\nfloat = %d\nstring = %d\nvector = %d\narray1 = %d\n\n", snek_len(inta), snek_len(floata), snek_len(stringa), snek_len(vectora), snek_len(arr)); 
	snek_object_t *intb = new_snek_integer(10); 
	snek_object_t* floatb = new_snek_float(9.1); 
	snek_object_t* stringb = new_snek_string(" qualcomm");
	snek_object_t* vectorb = new_snek_vector(intb, floatb, stringb);
	
	snek_object_t *arr2 = new_snek_array(4); 
	snek_array_set(arr2, 0, intb); 
	snek_array_set(arr2, 1, floatb); 
	snek_array_set(arr2, 2, stringb); 
	snek_array_set(arr2, 3, vectorb); 

	snek_object_t *intc = snek_add(inta, intb); 
	snek_object_t *floatc = snek_add(floata, floatb);
	snek_object_t *stringc = snek_add(stringa, stringb); 
	snek_object_t *vectorc = snek_add(vectora, vectorb);
	snek_object_t *arr3 = snek_add(arr, arr2); 

	printf("addition : \n"); 
	printf("%d\n", intc -> data.v_int); 
	printf("%f\n", floatc -> data.v_float);
	printf("%s\n", stringc -> data.v_string); 
	printf("x = %d\ny = %f\nz = %s\n\n", vectorc -> data.v_vector.x -> data.v_int,  vectorc -> data.v_vector.y -> data.v_float, vectorc -> data.v_vector.z -> data.v_string); 
	
	printf("array 3 : \n");
       	snek_array_t array3  = arr3 -> data.v_array; 	
	printf("%d\n", array3.elements[0] -> data.v_int); 
	printf("%f\n", array3.elements[1] -> data.v_float);
	printf("%s\n", array3.elements[2] -> data.v_string); 
	printf("x = %d\ny = %f\nz = %s\n\n", array3.elements[3] -> data.v_vector.x -> data.v_int,  array3.elements[3] -> data.v_vector.y -> data.v_float, array3.elements[3] -> data.v_vector.z -> data.v_string); 
	printf("%d\n", array3.elements[4] -> data.v_int); 
	printf("%f\n", array3.elements[5] -> data.v_float);
	printf("%s\n", array3.elements[6] -> data.v_string); 
	printf("x = %d\ny = %f\nz = %s\n\n", array3.elements[7] -> data.v_vector.x -> data.v_int,  array3.elements[7]  -> data.v_vector.y -> data.v_float, array3.elements[7] -> data.v_vector.z -> data.v_string); 

	return 0; 
}


