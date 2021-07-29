#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.h"

uint64_t power(int base, int exp) {
    uint64_t res = base;
    for (int i = 1; i < exp; i++) res *= base;
    return res;
}

void test_get_home_fp() {
    char *filename = "truc";
    char *home_filename = get_home_fp(filename);
    fprintf(stdout, "INFO: home filepath of \"%s\": %s\n", filename, home_filename);
    return;
}

void test_get_current_timestamp() {
    uint64_t current_timestamp = get_current_timestamp();
    fprintf(stdout, "TEST: get_current_timestamp(): %li\n", current_timestamp);
    float ct_div = (float)current_timestamp / power(10, 12);
    bool is_milliseconds = (ct_div > 1) && (ct_div < 10);
    if (is_milliseconds) fprintf(stdout, "INFO: timestamp size is correct\nTEST PASSED\n");
    else fprintf(stderr, "ERROR: timestamp is wrong, current_timestamp / 10^13 = %f\nTEST FAILED\n", ct_div);
    return;
}

void test_get_api_key() {
    bool secret = false;
    char *key;
    key = get_api_key(secret);
    printf("DEBUG: key = %s\n", key);
    if (strcmp(key, "") == 0)
        fprintf(stderr, "ERROR: failed to retrieve API key\nTEST FAILED\n");
    else 
        fprintf(stdout, "INFO: API key retrieved: %s\nTEST PASSED\n", key);

    secret = true;
    const char *secret_key;
    secret_key = get_api_key(secret);
    printf("DEBUG: secret_key = %s\n", secret_key);
    if (strcmp(secret_key, "") == 0)
        fprintf(stderr, "ERROR: failed to retrieve API secret key\nTEST FAILED\n");
    else 
        fprintf(stdout, "INFO: API secret key retrieved: %s\nTEST PASSED\n", key);
}


int main() {
    test_get_home_fp();
    test_get_current_timestamp();
    test_get_api_key();

    return EXIT_SUCCESS;
}
