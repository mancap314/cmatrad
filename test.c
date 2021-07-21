#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

uint64_t power(int base, int exp) {
    uint64_t res = base;
    for (int i = 1; i < exp; i++) res *= base;
    return res;
}

void test_get_current_timestamp() {
    uint64_t current_timestamp = get_current_timestamp();
    fprintf(stdout, "TEST: get_current_timestamp(): %li\n", current_timestamp);
    float ct_div = (float)current_timestamp / power(10, 12);
    bool is_milliseconds = (ct_div > 1) && (ct_div < 10);
    if (is_milliseconds) fprintf(stdout, "INFO: timestamp size is correct\n");
    else fprintf(stderr, "ERROR: timestamp is wrong, current_timestamp / 10^13 = %f\n", ct_div);
    return;
}


int main() {
    test_get_current_timestamp();
    return EXIT_SUCCESS;
}
