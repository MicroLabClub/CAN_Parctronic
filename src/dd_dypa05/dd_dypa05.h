#ifndef LIB_DYPA05_H_
#define LIB_DYPA05_H_

#include <stddef.h>

#define rxPin 5
#define txPin 6

uint16_t dd_dypa05_get_distance(size_t sensor_id);
void dd_dypa05_setup();
void dd_dypa05_loop();
void dd_dypa05_report();
    
#endif
