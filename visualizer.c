#include <stdio.h>                                                                                                                                                               
    #include <stdlib.h>                                                                                                                                                              
    #include "snekobject.h"                                                                                                                                                          
                                                                                                                                                                                     
    #ifdef USE_MARK_SWEEP                                                                                                                                                            
    #include "sneknew.h"                                                                                                                                                            
    #include "vm.h"                                                                                                                                                                  
    #else                                                                                                                                                                            
    #include "sneknew.h"                                                                                                                                                             
    #include "refcount.h"                                                                                                                                                            
    #endif                                                                                                                                                                           
                                                                                                                                                                                     
    int main() {                                                                                                                                                                     
    #ifdef USE_MARK_SWEEP                                                                                                                                                            
        printf("--- Running Mark & Sweep Heap Visualizer ---\n");                                                                                                                    
        vm_t *vm = vm_new();                                                                                                                                                         
        frame_t *frame = vm_new_frame(vm);                                                                                                                                           
                                                                                                                                                                                     
        // Allocate objects                                                                                                                                                          
        snek_object_t *inta = new_snek_integer(vm, 42);                                                                                                                              
        frame_reference_object(frame, inta);                                                                                                                                         
                                                                                                                                                                                     
        snek_object_t *floata = new_snek_float(vm, 3.14);                                                                                                                            
        frame_reference_object(frame, floata);                                                                                                                                       
                                                                                                                                                                                     
        snek_object_t *stringa = new_snek_string(vm, "visualize me");                                                                                                                
        frame_reference_object(frame, stringa);                                                                                                                                      
                                                                                                                                                                                     
        snek_object_t *vectora = new_snek_vector3(vm, inta, floata, stringa);                                                                                                        
        frame_reference_object(frame, vectora);                                                                                                                                      
                                                                                                                                                                                     
        printf("\nHeap layout (all references active in frame):");                                                                                                                   
        vm_visualize_heap(vm);                                                                                                                                                       
                                                                                                                                                                                     
        // Simulate scope exit by popping frame                                                                                                                                      
        printf("Popping stack frame (simulating variables going out of scope)...\n");                                                                                                
        frame_t *popped_frame = stack_pop(vm->frames);                                                                                                                               
        frame_free(popped_frame);                                                                                                                                                    
                                                                                                                                                                                     
        printf("\nHeap layout after variables go out of scope (unmarked):");                                                                                                         
        vm_visualize_heap(vm);                                                                                                                                                       
                                                                                                                                                                                     
        // Trigger GC sweep                                                                                                                                                          
        printf("Triggering GC sweep...\n");                                                                                                                                          
        vm_collect_garbage(vm);                                                                                                                                                      
                                                                                                                                                                                     
        printf("\nHeap layout after GC sweep (should be empty):");                                                                                                                   
        vm_visualize_heap(vm);                                                                                                                                                       
                                                                                                                                                                                     
        vm_free(vm);                                                                                                                                                                 
    #else                                                                                                                                                                            
        printf("--- Running Reference Counting Heap Visualizer ---\n");                                                                                                              
                                                                                                                                                                                     
        // Allocate objects                                                                                                                                                          
        snek_object_t *inta = new_snek_integer(42);                                                                                                                                  
        snek_object_t *floata = new_snek_float(3.14);                                                                                                                                
        snek_object_t *stringa = new_snek_string("visualize me");                                                                                                                    
        snek_object_t *vectora = new_snek_vector(inta, floata, stringa);                                                                                                             
                                                                                                                                                                                     
        printf("\nHeap layout (all allocations active):");                                                                                                                           
        rc_visualize_heap();                                                                                                                                                         
                                                                                                                                                                                     
        // Release the parent vector object                                                                                                                                          
        printf("Releasing vectora...\n");                                                                                                                                            
        refcount_dec(vectora);                                                                                                                                                       
                                                                                                                                                                                     
        printf("\nHeap layout after releasing vectora:");                                                                                                                            
        rc_visualize_heap();                                                                                                                                                         
                                                                                                                                                                                     
        // Release the standalone references                                                                                                                                         
        printf("Releasing individual components (inta, floata, stringa)...\n");                                                                                                      
        refcount_dec(inta);                                                                                                                                                          
        refcount_dec(floata);                                                                                                                                                        
        refcount_dec(stringa);                                                                                                                                                       
                                                                                                                                                                                     
        printf("\nHeap layout after releasing all references (should be empty):");                                                                                                   
        rc_visualize_heap();                                                                                                                                                         
    #endif                                                                                                                                                                           
        return 0;                                                                                                                                                                    
    } 
