/*
 * crc.c
 *
 *  Created on: Dec 9, 2025
 *      Author: anurag
 */

#include "stdio.h"
#include "crc.h"


void DATA_PEC(const uint8_t *data, uint16_t size, uint8_t *pec){
	uint16_t crc = (uint16_t)(CRC10_SEED & CRC10_MASK);

	for (uint16_t i =0; i < size; i++) {
		uint8_t b = data[i];
		for (int8_t bit = 7; bit >=0; bit--){
			uint8_t inbit = (b >> bit) & 1u;   //data bit
			uint8_t top = (uint8_t)((crc >> (CRC10_WIDTH - 1)) & 1u);	//crc msb
			uint8_t feedback = top ^ inbit;
			crc = (uint16_t)((crc << 1) & CRC10_MASK);
			if (feedback) {
				crc ^= (uint16_t)CRC10_POLY_MASK;
			}
		}
	}
	for (int8_t bit = 5; bit >=0; bit--){		//icluding 6 "0" of command counter while writing
		uint8_t inbit = 0u;
		uint8_t top = (uint8_t)((crc >> (CRC10_WIDTH - 1)) & 1u);
		uint8_t feedback = top ^ inbit;
		crc = (uint16_t)((crc << 1) & CRC10_MASK);
		if (feedback) {
			crc ^= (uint16_t)CRC10_POLY_MASK;
		}
	}
	crc &= CRC10_MASK ;

	pec[0] = (uint8_t)((crc >> 8) & 0x03u);
	pec[1] = (uint8_t)(crc & 0xFFu);
}


void COMMAND_PEC(const uint8_t *data, uint16_t size, uint8_t *pec){
	uint16_t crc = (uint16_t)(CRC15_SEED & CRC15_MASK);

	for (uint16_t i = 0;  i < size; i++){
		uint8_t b = data[i];
		for (int8_t bit = 7; bit >= 0; bit--){
			uint8_t inbit = (b >> bit) & 1u;	//data bit
			uint8_t top = (uint8_t)((crc >> (CRC15_WIDTH - 1)) & 1u);	//crc msb
			uint8_t feedback = top ^ inbit;
			crc = (uint16_t)((crc << 1) & CRC15_MASK);
			if (feedback) {
				crc ^= (uint16_t)CRC15_POLY_MASK;
			}
		}
	}
	crc &= CRC15_MASK ;
	pec[0] = (uint8_t)((crc >> 7) & 0xFFu);
	pec[1] = (uint8_t)((crc & 0x7Fu) << 1);
}

