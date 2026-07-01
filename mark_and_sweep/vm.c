#include "vm.h"

void sweep(vm_t* vm){
	if(vm == NULL) return ;
	for(int i = 0; i < (int)vm -> objects -> count; i++){
		snek_object_t* obj = vm -> objects -> data[i]; 
		if(obj -> is_marked) obj -> is_marked = false; 
		else {
			snek_object_free(obj); 
			vm -> objects -> data[i] = NULL; 
		}
	}
	stack_remove_nulls(vm -> objects); 
}

void vm_collect_garbage(vm_t* vm){
	if(vm == NULL) return; 
	mark(vm); 
	trace(vm); 
	sweep(vm); 
}

void mark(vm_t* vm){
	if(vm == NULL) return ;
	for(int i = 0; i < (int)vm -> frames -> count; i++){
		frame_t *frame = vm -> frames -> data[i]; 
		for(int j = 0; j < (int)frame -> references -> count; j++){
			snek_object_t* obj = (snek_object_t*) frame -> references -> data[j];
			if(obj == NULL) return;			
			obj -> is_marked = true; 
		} 
	}
}

void trace_mark_objects(stack_t *gray_objects, snek_object_t *obj){
	if(obj == NULL || obj -> is_marked) return; 
	obj -> is_marked = true; 
	stack_push(gray_objects,(void *) obj);
}

void trace_blacken_objects(stack_t *gray_objects, snek_object_t *obj){
	switch(obj -> kind){
		case INTEGER: break; 
		case FLOAT : break;
		case STRING : break;
		case VECTOR3: {
			      	trace_mark_objects(gray_objects, obj -> data.v_vector.x);
			      	trace_mark_objects(gray_objects, obj -> data.v_vector.y);
			      	trace_mark_objects(gray_objects, obj -> data.v_vector.z);
				break; 
			      }	      
		case ARRAY : {
				snek_array_t arr = obj -> data.v_array;
			     	for(int i = 0; i < (int)arr.size; i++) trace_mark_objects(gray_objects, snek_array_get(obj, i));
				break; 
			     }
	}

}

void trace(vm_t *vm){
	if(vm == NULL) return; 

	stack_t *gray_objects = stack_new(8); 
	if(gray_objects == NULL) return; 
	
	for(int i = 0; i < (int)vm -> objects -> count; i++){
		snek_object_t* obj = vm -> objects -> data[i]; 
		if(obj && obj -> is_marked) stack_push(gray_objects, obj);
	}
	
	while(gray_objects -> count > 0){
		snek_object_t* obj = (snek_object_t *)stack_pop(gray_objects); 
		trace_blacken_objects(gray_objects, obj);
	}

	stack_free(gray_objects); 
}

void vm_track_object(vm_t *vm, snek_object_t *obj){
	if(vm == NULL || obj == NULL) return; 
	stack_push(vm -> objects, (void *)obj); 
}

vm_t* vm_new(){
	vm_t* vm = malloc(sizeof(vm_t));
	if(vm == NULL) return NULL; 
 	
	vm -> frames = stack_new(8); 
	if(vm -> frames == NULL){
		free(vm); 
		return NULL;
	}	
	vm -> objects = stack_new(8); 
	if(vm -> objects == NULL){
		free(vm); 
		return NULL;
	}
	return vm;
}

void vm_free(vm_t* vm){
	if(vm == NULL) return ; 
	while(vm -> frames -> count != 0){
		frame_t *frame = stack_pop(vm -> frames); 
		frame_free(frame); 
	}
	stack_free(vm -> frames);
	while(vm -> objects -> count != 0){
		snek_object_t* obj = stack_pop(vm -> objects); 	
		snek_object_free(obj); 
	}
	stack_free(vm -> objects); 
	
	free(vm); 
}

void vm_frame_push(vm_t *vm, frame_t *frame){
	if(vm == NULL || frame == NULL) return; 
	stack_push(vm -> frames, frame); 
}

frame_t *vm_new_frame(vm_t *vm){
	frame_t* frame = malloc(sizeof(frame_t));
        if(frame == NULL) return NULL; 

	frame -> references = stack_new(8);
	if(frame -> references == NULL){
		free(frame); 
		return NULL; 
	}

	vm_frame_push(vm, frame); 
	return frame; 	
}


void frame_free(frame_t *frame){
	if(frame == NULL) return ; 
	stack_free(frame -> references); 
	free(frame); 	
}

void frame_reference_object(frame_t *frame, snek_object_t *obj){
	if(frame == NULL || obj == NULL) return ; 
	stack_push(frame -> references, (void *)obj); 
}
