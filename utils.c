#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"


uint64_t get_current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    /* timestamps all in millisenconds: seconds * 1000 + microseconds / 1000 */
    uint64_t current_timestamp =  (uint64_t) te.tv_sec * 1000 + te.tv_usec / 1000;
    return current_timestamp;
}
