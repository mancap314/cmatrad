#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <jansson.h>
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
    if (is_milliseconds) fputs("INFO: timestamp size is correct\nTEST PASSEDi\n", stdout);
    else fprintf(stderr, "ERROR: timestamp is wrong, current_timestamp / 10^13 = %f\nTEST FAILED\n", ct_div);
    return;
}


void test_get_api_key() {
    bool secret = false;
    const char *key;
    key = get_api_key(secret);
    if (strcmp(key, "") == 0)
        fputs("ERROR: failed to retrieve API key\nTEST FAILED\n", stderr);
    else 
        fprintf(stdout, "INFO: API key retrieved: %s\nTEST PASSED\n", key);

    secret = true;
    const char *secret_key;
    secret_key = get_api_key(secret);
    printf("DEBUG: secret_key = %s\n", secret_key);
    if (strcmp(secret_key, "") == 0)
        fputs("ERROR: failed to retrieve API secret key\nTEST FAILEDi\n", stderr) ;
    else 
        fprintf(stdout, "INFO: API secret key retrieved: %s\nTEST PASSED\n", key);
    return;
}


void test_get_query_string() {
    // 1. Create a jansson key=>value map
    json_t *kvm = json_object();
    json_object_set_new(kvm, "k1", json_string("v1"));
    json_object_set_new(kvm, "k2", json_string("v2"));
    json_object_set_new(kvm, "k3", json_string("v3"));
    // 2. Test get_query_string() on this map
    char *query_str = get_query_string(kvm);
    char *expected_query_str = "k1=v1&k2=v2&k3=v3";
    fprintf(stdout, "INFO: query_str: \"%s\", expected: \"%s\"\n", query_str, expected_query_str);
    if (strcmp(query_str, expected_query_str) == 0)
        puts("INFO: Test for get_query_string(): TEST PASSED");
    else fputs("ERROR: Test for get_query_string(): TEST FAILED\n", stderr);
    json_decref(0);
    return;
}


void test_get_hashmap_signature() {
    json_t *params = json_object();
    json_t *data = json_object();
    json_object_set_new(params, "pk1", json_string("pv1"));
    json_object_set_new(data, "dk1", json_string("dv1"));
    json_object_set_new(data, "dk2", json_string("dv2"));
   
    const char *secret_key = get_api_key(true);
    char *hashmap_signature = get_hashmap_signature(params, data, secret_key);
    char *expected_hashmap_signature = "a0246cd3ab8d2ba7f4f74d12dd083ebac2ff2be50c329a58be46a74b37a1afb1";

    fprintf(stdout, "INFO: hashmap signature computed:\n%s\nExpected:\n%s\n", hashmap_signature, expected_hashmap_signature);
    if (strcmp(hashmap_signature, expected_hashmap_signature) == 0)
        puts("INFO: get_hashmap_signature(): TEST PASSED");
    else fputs("ERROR: get_hashmap_signature(): TEST FAILEDi\n", stderr);
    return;
}

int main() {
    test_get_home_fp();
    test_get_current_timestamp();
    test_get_api_key();
    test_get_query_string();
    test_get_hashmap_signature();

    return EXIT_SUCCESS;
}
