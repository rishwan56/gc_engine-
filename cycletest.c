#include <stdio.h>
#include "refcount.h"
#include "snekobject.h"
#include "sneknew.h" 

void run_acyclic_test() {
	printf("--- RUNNING ACYCLIC TEST ---\n");
	snek_object_t *a = new_snek_array(2); 
	snek_object_t *b = new_snek_array(2); 
	
	printf("Initial refcounts:\n");
	printf("  a refcount: %d (expected: 1)\n", a->refcount);
	printf("  b refcount: %d (expected: 1)\n", b->refcount);

	printf("Setting a[1] = b...\n");
	snek_array_set(a, 1, b); 
	printf("Refcounts after linking (acyclic):\n");
	printf("  a refcount: %d (expected: 1)\n", a->refcount);
	printf("  b refcount: %d (expected: 2)\n", b->refcount);

	printf("Releasing stack reference to a (refcount_dec(a))...\n");
	refcount_dec(a); 
	printf("Releasing stack reference to b (refcount_dec(b))...\n");
	refcount_dec(b); 
	printf("--- ACYCLIC TEST FINISHED ---\n\n");
}

void run_cyclic_test() {
	printf("--- RUNNING CYCLIC TEST ---\n");
	snek_object_t *a = new_snek_array(2); 
	snek_object_t *b = new_snek_array(2); 

	printf("Initial refcounts:\n");
	printf("  a refcount: %d (expected: 1)\n", a->refcount);
	printf("  b refcount: %d (expected: 1)\n", b->refcount);

	printf("Setting a[1] = b...\n");
	snek_array_set(a, 1, b); 
	printf("Setting b[1] = a...\n");
	snek_array_set(b, 1, a); 

	printf("Refcounts after linking (cyclic):\n");
	printf("  a refcount: %d (expected: 2)\n", a->refcount);
	printf("  b refcount: %d (expected: 2)\n", b->refcount);

	printf("Releasing stack reference to a (refcount_dec(a))...\n");
	refcount_dec(a);
	printf("  a refcount: %d (expected: 1)\n", a->refcount);

	printf("Releasing stack reference to b (refcount_dec(b))...\n");
	refcount_dec(b);
	printf("  b refcount: %d (expected: 1)\n", b->refcount);

	printf("\nNotice: [GC] Freeing messages were NOT printed for the cyclic objects!\n");
	printf("Even though we have no more stack references to 'a' or 'b', they are still alive in memory because their refcounts are 1.\n");
	printf("--- CYCLIC TEST FINISHED ---\n");
}

int main() {
	run_acyclic_test();
	run_cyclic_test();
	return 0;
}
