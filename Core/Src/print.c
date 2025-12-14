/*
 * print.c
 *
 *  Created on: Dec 11, 2025
 *      Author: anurag
 */

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "main.h"

extern UART_HandleTypeDef hlpuart1;

void print_hex(const char *label, const uint8_t *data, uint16_t size){
	char msg[200];
	int pos = 0;
	 // Print label first (e.g., "CFGA = ")
	pos += snprintf(msg + pos, sizeof(msg) - pos, "%s = ", label);

	// Append all bytes
	for (uint16_t i = 0; i < size; i++) {
		pos += snprintf(msg + pos, sizeof(msg) - pos, "%02X ", data[i]);
		if (pos >= sizeof(msg)) break;  // safety check
	}

	// Append newline
	pos += snprintf(msg + pos, sizeof(msg) - pos, "\r\n");

	// Transmit
	HAL_UART_Transmit(&hlpuart1, (uint8_t *)msg, pos, 10);
}

