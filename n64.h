#ifndef N64_H
#define N64_H

#include <stdint.h>
#include <stdbool.h>

#define NTD_QUERY_CONTROLLER_TYPE 0x00
#define NTD_HIGH_CYCLES 30

#define N64_QUERY_BUTTONS 0x01

#define GC_QUERY_BUTTONS_0 0x40
#define GC_QUERY_BUTTONS_1 0x03
#define GC_QUERY_BUTTONS_2 0x00

#define UNKNOWN_CONTROLLER 0
#define N64_CONTROLLER 1
#define GC_CONTROLLER 2

uint8_t ntd_request(uint8_t *request, uint8_t request_length, uint8_t response[128], uint8_t portc_mask);
uint8_t ntd_controller_type(uint8_t response[128], int length);
bool n64_parse_response(uint8_t response[128], int length, uint16_t *buttons, int8_t *analog_x, int8_t *analog_y);
bool gc_parse_response(uint8_t response[128], int length, uint16_t *buttons, int8_t *left_x, int8_t *left_y, int8_t *left_z, int8_t *right_x, int8_t *right_y, int8_t *right_z);

#endif /* N64_H */

