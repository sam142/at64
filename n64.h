#ifndef N64_H
#define N64_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_RESPONSE_SIZE 128

#define NTD_QUERY_CONTROLLER_TYPE 0x00
#define NTD_HIGH_CYCLES 30

#define N64_QUERY_BUTTONS 0x01

#define GC_QUERY_BUTTONS_0 0x40
#define GC_QUERY_BUTTONS_1 0x03
#define GC_QUERY_BUTTONS_2 0x00

#define UNKNOWN_CONTROLLER 0
#define N64_CONTROLLER 1
#define GC_CONTROLLER 2

struct __attribute__((__packed__)) gc_controller_state
{
    int8_t left_x;
    int8_t left_y;
    int8_t right_x;
    int8_t right_y;
	uint16_t buttons;
};

struct __attribute__((__packed__)) n64_controller_state
{
    int8_t left_x;
    int8_t left_y;
	uint16_t buttons;
};

uint8_t ntd_request(uint8_t *request, uint8_t request_length, uint8_t response[MAX_RESPONSE_SIZE], uint8_t portc_mask);
uint8_t ntd_controller_type(uint8_t response[MAX_RESPONSE_SIZE], int length);
bool n64_parse_response(uint8_t response[MAX_RESPONSE_SIZE], int length, struct n64_controller_state *state);
bool gc_parse_response(uint8_t response[MAX_RESPONSE_SIZE], int length, struct gc_controller_state *state);

#endif /* N64_H */

