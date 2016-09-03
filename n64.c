#include "n64.h"

bool n64_parse_response(uint8_t response[MAX_RESPONSE_SIZE], int length, struct n64_controller_state *state)
{
	int j;

	if(length != 33)
		return false;

    *state = (const struct n64_controller_state){ 0 };

	for(j=0; j<=15; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->buttons |= (1<<j);

	for(; j<=23; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->left_x |= (1<<(23-j));

	for(; j<=31; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->left_y |= (1<<(31-j));

	return true;
}

bool gc_parse_response(uint8_t response[MAX_RESPONSE_SIZE], int length, struct gc_controller_state *state)
{
	int j;

	if(length != 65)
		return false;

    *state = (const struct gc_controller_state){ 0 };

	for(j=3; j<=15; j++)
		if(response[j] < NTD_HIGH_CYCLES && j != 8)
			state->buttons |= (1<<(j-3));

	for(; j<=23; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->left_x  |= (1<<(23-j));

	for(; j<=31; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->left_y |= (1<<(31-j));

	for(; j<=39; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->right_x |= (1<<(39-j));

	for(; j<=47; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->right_y |= (1<<(47-j));

    /* TODO
	for(; j<=55; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->left_z |= (1<<(55-j));

	for(; j<=63; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			state->right_z |= (1<<(63-j));
    */

	return true;
}

uint8_t ntd_controller_type(uint8_t response[MAX_RESPONSE_SIZE], int length)
{
	if(length != 25)
		return UNKNOWN_CONTROLLER;

	if(response[5] < NTD_HIGH_CYCLES)
		return N64_CONTROLLER;
	else
		return GC_CONTROLLER;
}

