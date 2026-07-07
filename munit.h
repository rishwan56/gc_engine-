#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    const char *name;
    void (*test)(void);
} MunitTest;

typedef struct {
    const char *name;
    MunitTest *tests;
} MunitSuite;

#define RUN
#define SUBMIT

#define munit_case(mode, name, body) \
    void name(void) body

#define munit_test(path, fn) \
    { path, fn }

#define munit_null_test \
    { NULL, NULL }

#define munit_suite(name, tests) \
    { name, tests }

#define assert_true(x) assert(x)
#define assert_false(x) assert(!(x))

#define assert_int(a, op, b, msg) \
    do { \
        if (!((a) op (b))) { \
            fprintf(stderr, "%s\n", msg); \
            assert((a) op (b)); \
        } \
    } while (0)

#define assert_ptr_not_null(ptr, msg) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "%s\n", msg); \
            assert((ptr) != NULL); \
        } \
    } while (0)

static inline int munit_suite_main(
    MunitSuite *suite,
    void *user_data,
    int argc,
    char **argv
) {
    (void)user_data;
    (void)argc;
    (void)argv;

    printf("Running suite: %s\n", suite->name);

    for (int i = 0; suite->tests[i].name != NULL; i++) {
        printf("Running %s ... ", suite->tests[i].name);
        suite->tests[i].test();
        printf("PASS\n");
    }

    printf("All tests passed\n");
    return 0;
}
