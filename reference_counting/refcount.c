#include "refcount.h" 
#include <stdlib.h>
#include <stdio.h>


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
	if(obj -> prev) obj -> prev -> next = obj -> next; 
	else global_head = obj -> next; 
	if(obj -> next) obj -> next -> prev = obj -> prev; 
	free(obj); 				
	total_live_allocation--; 
}

void rc_visualize_heap(void) {                                                                                                                                                   
        printf("\n=== Heap Visualizer (Reference Counting) ===\n");                                                                                                                  
        snek_object_t *curr = global_head;                                                                                                                                           
        size_t count = 0;                                                                                                                                                            
        while (curr != NULL) {                                                                                                                                                       
            printf("[%p] ", (void *)curr);                                                                                                                                           
            switch (curr->kind) {                                                                                                                                                    
                case INTEGER:                                                                                                                                                        
                    printf("INTEGER: %d", curr->data.v_int);                                                                                                                         
                    break;                                                                                                                                                           
                case FLOAT:                                                                                                                                                          
                    printf("FLOAT: %f", curr->data.v_float);
                    break;
                case STRING:
                    printf("STRING: \"%s\"", curr->data.v_string);
                    break;
                case VECTOR3:
                    printf("VECTOR3: { x: %p, y: %p, z: %p }", 
                           (void *)curr->data.v_vector3.x, 
                           (void *)curr->data.v_vector3.y, 
                           (void *)curr->data.v_vector3.z);
                    break;
                case ARRAY:
                    printf("ARRAY (size %zu): [", curr->data.v_array.size);
                    for (size_t j = 0; j < curr->data.v_array.size; j++) {
                        printf("%p%s", (void *)curr->data.v_array.elements[j], 
                               j == curr->data.v_array.size - 1 ? "" : ", ");
                    }
                    printf("]");
                    break;
            }
            printf(" | Refcount: %d\n", curr->refcount);
            curr = curr->next;
            count++;
        }
        printf("Total Live Objects: %zu\n", count);
        printf("===========================================\n\n");
}
void rc_report_leaks(void) {                                                                                                                                                     
        if (total_live_allocation == 0) {                                                                                                                                            
            printf("\n[Leak Detector] Clean exit! 0 leaks detected.\n");                                                                                                             
            return;                                                                                                                                                                  
        }                                                                                                                                                                            
                                                                                                                                                                                     
        printf("\n[Leak Detector] WARNING: %d memory leaks detected!\n", total_live_allocation);                                                                                     
        printf("--------------------------------------------------\n");                                                                                                              
        snek_object_t *curr = global_head;                                                                                                                                           
        while (curr != NULL) {                                                                                                                                                       
            printf("Leaked Object [%p] | Type: ", (void *)curr);                                                                                                                     
            switch (curr->kind) {                                                                                                                                                    
                case INTEGER: printf("INTEGER (%d)", curr->data.v_int); break;                                                                                                       
                case FLOAT:   printf("FLOAT (%f)", curr->data.v_float); break;                                                                                                       
                case STRING:  printf("STRING (\"%s\")", curr->data.v_string); break;                                                                                                 
                case VECTOR3:                                                                                                                                                        
                    printf("VECTOR3 { x: %p, y: %p, z: %p }",                                                                                                                        
                           (void *)curr->data.v_vector3.x,                                                                                                                           
                           (void *)curr->data.v_vector3.y,                                                                                                                           
                           (void *)curr->data.v_vector3.z);                                                                                                                          
                    break;                                                                                                                                                           
                case ARRAY:                                                                                                                                                          
                    printf("ARRAY (size %zu)", curr->data.v_array.size);                                                                                                             
                    break;                                                                                                                                                           
            }                                                                                                                                                                        
            printf(" | Refcount: %d\n", curr->refcount);                                                                                                                             
            curr = curr->next;                                                                                                                                                       
        }                                                                                                                                                                            
        printf("--------------------------------------------------\n\n");                                                                                                            
}  
