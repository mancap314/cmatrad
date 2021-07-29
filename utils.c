#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <jansson.h>
#include <string.h>
#include "utils.h"


/* Get the path of a file relatively to the home directory */
char* get_home_fp(char *filename) {
   char *home = getenv (HOMEENV);
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
