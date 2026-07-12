
#include "sneknew.h"                                                                                                                                                             
    #include "snekobject.h"                                                                                                                                                          
    #include "vm.h"                                                                                                                                                                  
    #include <stdio.h>                                                                                                                                                               
    #include <stdlib.h>                                                                                                                                                              
                                                                                                                                                                                     
    // Forward declaration of snek_add in case it's not declared in headers                                                                                                          
    snek_object_t *snek_add(vm_t *vm, snek_object_t *a, snek_object_t *b);                                                                                                           
                                                                                                                                                                                     
    // Global allocator counter (from sneknew.c / sneknew.h)                                                                                                                         
    extern int total_live_allocation;                                                                                                                                                
                                                                                                                                                                                     
    int main() {                                                                                                                                                                     
	printf("mark and sweep \n");
        // 1. Initialize the VM                                                                                                                                                      
        vm_t *vm = vm_new();                                                                                                                                                         
                                                                                                                                                                                     
        // 2. Create a stack frame (represents the local scope of main)                                                                                                              
        frame_t *frame = vm_new_frame(vm);                                                                                                                                           
                                                                                                                                                                                     
        // Integer check                                                                                                                                                             
        snek_object_t *inta = new_snek_integer(vm, 5);                                                                                                                               
        frame_reference_object(frame, inta); // Register in frame so GC knows it's a live local variable                                                                             
        printf("int: kind = %d, data = %d\n", inta->kind, inta->data.v_int);                                                                                                         
        printf("total alive objects = %d\n\n", total_live_allocation);                                                                                                               
                                                                                                                                                                                     
        // Float check                                                                                                                                                               
        snek_object_t *floata = new_snek_float(vm, 3.14);                                                                                                                            
        frame_reference_object(frame, floata);                                                                                                                                       
        printf("float: kind = %d, data = %f\n", floata->kind, floata->data.v_float);                                                                                                 
        printf("total alive objects = %d\n\n", total_live_allocation);                                                                                                               
                                                                                                                                                                                     
        // String check                                                                                                                                                              
        snek_object_t *stringa = new_snek_string(vm, "rishwan");                                                                                                                     
        frame_reference_object(frame, stringa);                                                                                                                                      
        printf("string: kind = %d, data = %s\n", stringa->kind, stringa->data.v_string);                                                                                             
        printf("total alive objects = %d\n\n", total_live_allocation);                                                                                                               
                                                                                                                                                                                     
        // Vector3 check                                                                                                                                                             
        snek_object_t *vectora = new_snek_vector3(vm, inta, floata, stringa);                                                                                                        
        frame_reference_object(frame, vectora);                                                                                                                                      
        printf("vector3: kind = %d\n", vectora->kind);                                                                                                                               
        printf("total alive objects = %d\n\n", total_live_allocation);                                                                                                               
                                                                                                                                                                                     
        // Array check                                                                                                                                                               
        snek_object_t *arr = new_snek_array(vm, 4);                                                                                                                                  
        frame_reference_object(frame, arr);                                                                                                                                          
        snek_array_set(arr, 0, inta);                                                                                                                                                
        snek_array_set(arr, 1, floata);                                                                                                                                              
        snek_array_set(arr, 2, stringa);                                                                                                                                             
        snek_array_set(arr, 3, vectora);                                                                                                                                             
        printf("array: kind = %d\n", arr->kind);                                                                                                                                     
        printf("total alive objects = %d\n\n", total_live_allocation);                                                                                                               
                                                                                                                                                                                     
        // Second group                                                                                                                                                              
        snek_object_t *intb = new_snek_integer(vm, 10);                                                                                                                              
        frame_reference_object(frame, intb);                                                                                                                                         
        snek_object_t *floatb = new_snek_float(vm, 9.1);                                                                                                                             
        frame_reference_object(frame, floatb);                                                                                                                                       
        snek_object_t *stringb = new_snek_string(vm, " qualcomm");                                                                                                                   
        frame_reference_object(frame, stringb);                                                                                                                                      
        snek_object_t *vectorb = new_snek_vector3(vm, intb, floatb, stringb);                                                                                                        
        frame_reference_object(frame, vectorb);                                                                                                                                      
        snek_object_t *arr2 = new_snek_array(vm, 4);                                                                                                                                 
        frame_reference_object(frame, arr2);                                                                                                                                         
        snek_array_set(arr2, 0, intb);                                                                                                                                               
        snek_array_set(arr2, 1, floatb);                                                                                                                                             
        snek_array_set(arr2, 2, stringb);                                                                                                                                            
        snek_array_set(arr2, 3, vectorb);                                                                                                                                            
        printf("total alive objects after 2nd group = %d\n\n", total_live_allocation);                                                                                               
                                                                                                                                                                                     
        // Additions                                                                                                                                                                 
        snek_object_t *intc = snek_add(vm, inta, intb);                                                                                                                              
        frame_reference_object(frame, intc);                                                                                                                                         
        snek_object_t *floatc = snek_add(vm, floata, floatb);                                                                                                                        
        frame_reference_object(frame, floatc);                                                                                                                                       
        snek_object_t *stringc = snek_add(vm, stringa, stringb);                                                                                                                     
        frame_reference_object(frame, stringc);                                                                                                                                      
        snek_object_t *vectorc = snek_add(vm, vectora, vectorb);                                                                                                                     
        frame_reference_object(frame, vectorc);                                                                                                                                      
        snek_object_t *arr3 = snek_add(vm, arr, arr2);                                                                                                                               
        frame_reference_object(frame, arr3);                                                                                                                                         
        printf("total alive objects after additions = %d\n\n", total_live_allocation);                                                                                               
  
        // Print addition outputs
        printf("addition:\n");
        printf("%d\n", intc->data.v_int);
        printf("%f\n", floatc->data.v_float);
        printf("%s\n", stringc->data.v_string);
  
        // ----------------------------------------------------
        // CLEANING UP WITH GARBAGE COLLECTOR (Mark & Sweep)
        // ----------------------------------------------------
        printf("Before GC (all references active in frame):\n");
        printf("  total alive objects = %d\n\n", total_live_allocation);
  
        // Simulate local scope ending: Pop and free the stack frame.
        // This leaves all allocated objects on the heap but with no roots pointing to them.
        printf("Popping stack frame (simulating variables going out of scope)...\n");
        frame_t *popped_frame = stack_pop(vm->frames);
        frame_free(popped_frame);
  
        // Trigger GC sweep
        printf("Running garbage collector...\n");
        vm_collect_garbage(vm);
  
        printf("After GC:\n");
        printf("  total alive objects = %d (expected: 0)\n\n", total_live_allocation);
  
        // Finally free the VM structure
        vm_free(vm);
        
        printf("all working fine\n");
        return 0;
    }
