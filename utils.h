#ifndef UTILS_H
#define UTILS_H

#if defined (_WIN32) || defined (_WIN64)
     #define HAVEWIN 1
#elif defined (__unix__)
     #define HAVEUNIX 1
#endif

#ifdef HAVEWIN
     #define HOMEENV "USERPROFILE"
#elif HAVEUNIX
     #define HOMEENV "HOME"
#endif


#include <stdint.h>
#include <stdbool.h>
#include <jansson.h>

uint64_t get_current_timestamp();
const char* get_api_key(bool secret);
char* get_home_fp(char *filename);
char *get_query_string(json_t *params);

#endif
