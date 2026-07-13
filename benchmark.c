#include <stdio.h>                                                                                                                                                               
#include <stdlib.h>                                                                                                                                                              
#include <time.h>                                                                                                                                                                
#include "sneknew.h"                                                                                                                                                             
#include "snekobject.h"                                                                                                                                                          
                                                                                                                                                                                     
#ifdef USE_MARK_SWEEP                                                                                                                                                            
#include "vm.h"                                                                                                                                                                  
#else                                                                                                                                                                            
#include "refcount.h"                                                                                                                                                            
#endif                                                                                                                                                                           
                                                                                                                                                                                     
// Extern variable to track allocations                                                                                                                                          
extern int total_live_allocation;                                                                                                                                                
                                                                                                                                                                                     
#define ITERATIONS 100000                                                                                                                                                        
                                                                                                                                                                                     
void run_benchmark() {                                                                                                                                                           
	clock_t start = clock();                                                                                                                                                     
                                                                                                                                                                                     
#ifdef USE_MARK_SWEEP                                                                                                                                                            
        vm_t *vm = vm_new();                                                                                                                                                         
        frame_t *frame = vm_new_frame(vm);                                                                                                                                           
#endif                                                                                                                                                                           
                                                                                                                                                                                     
        int peak_memory = 0;                                                                                                                                                         
                                                                                                                                                                                     
        for (int i = 0; i < ITERATIONS; i++) {                                                                                                                                       
            // --- 1. ALLOCATION WORKLOAD ---                                                                                                                                        
#ifdef USE_MARK_SWEEP                                                                                                                                                            
            snek_object_t *arr = new_snek_array(vm, 3);                                                                                                                              
            frame_reference_object(frame, arr);                                                                                                                                      
                                                                                                                                                                                     
            snek_object_t *inta = new_snek_integer(vm, i);                                                                                                                           
            frame_reference_object(frame, inta);                                                                                                                                     
            snek_array_set(arr, 0, inta);                                                                                                                                            
                                                                                                                                                                                     
            snek_object_t *floata = new_snek_float(vm, (float)i * 1.5f);                                                                                                             
            frame_reference_object(frame, floata);                                                                                                                                   
            snek_array_set(arr, 1, floata);                                                                                                                                          
                                                                                                                                                                                     
            snek_object_t *stringa = new_snek_string(vm, "bench");                                                                                                                   
            frame_reference_object(frame, stringa);                                                                                                                                  
            snek_array_set(arr, 2, stringa);                                                                                                                                         
#else                                                                                                                                                                            
            snek_object_t *arr = new_snek_array(3);                                                                                                                                  
            snek_object_t *inta = new_snek_integer(i);                                                                                                                               
            snek_array_set(arr, 0, inta);                                                                                                                                            
                                                                                                                                                                                     
            snek_object_t *floata = new_snek_float((float)i * 1.5f);                                                                                                                 
            snek_array_set(arr, 1, floata);                                                                                                                                          
                                                                                                                                                                                     
            snek_object_t *stringa = new_snek_string("bench");                                                                                                                       
            snek_array_set(arr, 2, stringa);                                                                                                                                         
#endif                                                                                                                                                                           
                                                                                                                                                                                     
            // Track Peak Memory Usage                                                                                                                                               
            if (total_live_allocation > peak_memory) {                                                                                                                               
                peak_memory = total_live_allocation;                                                                                                                                 
            }                                                                                                                                                                        
                                                                                                                                                                                     
            // --- 2. DEALLOCATION / GC WORKLOAD ---                                                                                                                                 
#ifdef USE_MARK_SWEEP                                                                                                                                                            
            // For Mark & Sweep, run garbage collection every 1000 iterations                                                                                                        
            if (i % 1000 == 0) {                                                                                                                                                     
                frame_t *popped = stack_pop(vm->frames);                                                                                                                             
                frame_free(popped);                                                                                                                                                  
                                                                                                                                                                                     
                vm_collect_garbage(vm);                                                                                                                                              
                                                                                                                                                                                     
                frame = vm_new_frame(vm);                                                                                                                                            
            }                                                                                                                                                                        
#else                                                                                                                                                                            
            // For Reference Counting, decrement objects immediately                                                                                                                 
            refcount_dec(arr);                                                                                                                                                       
            refcount_dec(inta);                                                                                                                                                      
            refcount_dec(floata);                                                                                                                                                    
            refcount_dec(stringa);                                                                                                                                                   
#endif                                                                                                                                                                           
        }                                                                                                                                                                            
                                                                                                                                                                                     
#ifdef USE_MARK_SWEEP                                                                                                                                                            
        // Final clean up of VM                                                                                                                                                      
        vm_free(vm);                                                                                                                                                                 
#endif                                                                                                                                                                           
                                                                                                                                                                                     
        clock_t end = clock();                                                                                                                                                       
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;                                                                                                                  
                                                                                                                                                                                     
        printf("=========================================\n");                                                                                                                       
#ifdef USE_MARK_SWEEP                                                                                                                                                            
        printf(" BENCHMARK: Mark and Sweep Garbage Collector\n");                                                                                                                    
#else                                                                                                                                                                            
        printf(" BENCHMARK: Reference Counting\n");                                                                                                                                  
#endif                                                                                                                                                                           
        printf("=========================================\n");                                                                                                                       
        printf("Total Iterations:  %d\n", ITERATIONS);                                                                                                                               
        printf("Execution Time:    %.4f seconds\n", time_spent);                                                                                                                     
        printf("Peak Live Objects: %d\n", peak_memory);                                                                                                                              
        printf("End Live Objects:  %d\n", total_live_allocation);                                                                                                                    
        printf("=========================================\n");                                                                                                                       
                                                                                                                                                                                     
        // Save results to file for side-by-side comparison                                                                                                                          
#ifdef USE_MARK_SWEEP                                                                                                                                                            
        FILE *f = fopen("marksweep_bench.txt", "w");                                                                                                                                 
        if (f) {                                                                                                                                                                     
            fprintf(f, "%.4f %d\n", time_spent, peak_memory);                                                                                                                        
            fclose(f);                                                                                                                                                               
        }                                                                                                                                                                            
#else                                                                                                                                                                            
        FILE *f = fopen("refcount_bench.txt", "w");                                                                                                                                  
        if (f) {                                                                                                                                                                     
            fprintf(f, "%.4f %d\n", time_spent, peak_memory);                                                                                                                        
            fclose(f);                                                                                                                                                               
        }                                                                                                                                                                            
#endif                                                                                                                                                                           
    }                                                                                                                                                                                
                                                                                                                                                                                     
    int main() {                                                                                                                                                                     
        run_benchmark();                                                                                                                                                             
        return 0;                                                                                                                                                                    
    } 
