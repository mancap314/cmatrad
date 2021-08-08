#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <jansson.h>
#include <string.h>
#include <openssl/hmac.h>
#include "utils.h"


/* Get the path of a file relatively to the home directory */
char* get_home_fp(char *filename) {
   char *home = getenv(HOMEENV);
   if (!home) {
        fprintf (stderr, "ERROR: user home environment not found.\n");
        return "";
    }

    #ifdef HAVEWIN
    char *sep_char = "\\";
    #elif HAVEUNIX
    char *sep_char = "/";
    #endif
    
    char *home_filename;
    home_filename = (char *) malloc(strlen(home) + strlen(filename) + 1);
    strcpy(home_filename, home);
    strcat(home_filename, sep_char);
    strcat(home_filename, filename);

   return home_filename;
}


uint64_t get_current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    /* timestamps all in milliseconds: seconds * 1000 + microseconds / 1000 */
    uint64_t current_timestamp =  (uint64_t) te.tv_sec * 1000 + te.tv_usec / 1000;
    return current_timestamp;
}


const char* get_api_key(bool secret) {
    char *secret_fn = ".binance_api_secrets";
    const char *secret_path = get_home_fp(secret_fn);
    
    json_t *obj, *key_node;
    json_error_t err;
    const char *key, *value_str;
    
    obj = json_load_file(secret_path, 0, &err);
    if(!obj)
    {
        fprintf(stderr, "ERROR: can't read %s: line %d:\n%s\n", secret_path, err.line, err.text);
        return "";
    }

    if (secret)
    {
        key_node = json_object_get(obj, "secret");
    }
    else key_node = json_object_get(obj, "key");

    if(!json_is_string(key_node))
    {
        fprintf(stderr, "ERROR: the key is not a string\n");
        return "";
    }
    key = json_string_value(key_node);
    /* don't json_decref(obj), otherwise key is not anymore accessible */
    return key;
}


char *get_query_string(json_t *params) {
    char *query_str = (char*) malloc(sizeof(char));
    query_str[0] = '\0';
    int niter = 0;
    void *iter;
    const char *key_str, *value_str;
    json_t *value;

    iter = json_object_iter(params);
    while(iter) {
        key_str = json_object_iter_key(iter);
        value = json_object_iter_value(iter);
        value_str = json_string_value(value);
        
        if(realloc(query_str, sizeof(char) * (strlen(query_str) + strlen(key_str) + strlen(value_str) + 1 + niter)) != NULL)
        {
            if (niter > 0) strcat(query_str, "&");
            strcat(query_str, key_str);
            strcat(query_str, "=");
            strcat(query_str, value_str);
        }
        else fputs("ERROR: get_query_string(): failed to realloc memory for query_str\n", stderr);

        if (niter == 0) niter++;
        iter = json_object_iter_next(params, iter);
    }
    return query_str;
}


char *encryptWithHMAC(const char* key, const unsigned char* data) {
    unsigned char *result;
    static char res_hexstring[64];
    int result_len = 32;
    char *signature;

    result = HMAC(EVP_sha256(), key, strlen((char *)key), data, strlen((char *)data), NULL, NULL);
  	for (int i = 0; i < result_len; i++) {
    	sprintf(&(res_hexstring[i * 2]), "%02x", result[i]);
  	}

  	return res_hexstring;
}

char *get_hashmap_signature(json_t *params, json_t *data, const char *secret_key) {
    // Concatenate `params`and `data` query strings
    char *query_str = get_query_string(params);
    char *data_query_str = get_query_string(data);
    strcat(query_str, data_query_str);
    char *signature = encryptWithHMAC(secret_key, query_str);
    return signature;
}
