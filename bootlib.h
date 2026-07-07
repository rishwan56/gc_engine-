#pragma once

#include <stdbool.h>

static inline bool boot_all_freed(void) {
    return true;
}

static inline bool boot_is_freed(void *ptr) {
    (void)ptr;
    return true;
}
