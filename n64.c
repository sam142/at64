#include "n64.h"

bool n64_parse_response(uint8_t response[128], int length, uint16_t *buttons, int8_t *analog_x, int8_t *analog_y)
{
	int j;

	if(length != 33)
		return false;

	*buttons = *analog_x = *analog_y = 0;

	for(j=0; j<=15; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*buttons |= (1<<j);

	for(; j<=23; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*analog_x |= (1<<(23-j));

	for(; j<=31; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*analog_y |= (1<<(31-j));
			
	return true;	
}

bool gc_parse_response(uint8_t response[128], int length, uint16_t *buttons, int8_t *left_x, int8_t *left_y, int8_t *left_z, int8_t *right_x, int8_t *right_y, int8_t *right_z)
{
	int j;

	if(length != 65)
		return false;

	*buttons = *left_x = *left_y = *left_z = *right_x = *right_y = *right_z = 0;

	for(j=3; j<=15; j++)
		if(response[j] < NTD_HIGH_CYCLES && j != 8)
			*buttons |= (1<<(j-3));

	for(; j<=23; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*left_x  |= (1<<(23-j));

	for(; j<=31; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*left_y |= (1<<(31-j));

	for(; j<=39; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*right_x |= (1<<(39-j));

	for(; j<=47; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*right_y |= (1<<(47-j));

	for(; j<=55; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*left_z |= (1<<(55-j));

	for(; j<=63; j++)
		if(response[j] < NTD_HIGH_CYCLES)
			*right_z |= (1<<(63-j));
			
	return true;	
}

uint8_t ntd_controller_type(uint8_t response[128], int length)
{
	if(length != 25)
		return UNKNOWN_CONTROLLER;

	if(response[5] < NTD_HIGH_CYCLES)
		return N64_CONTROLLER;
	else
		return GC_CONTROLLER;
}

