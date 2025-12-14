/*
 * crc.h
 *
 *  Created on: Dec 9, 2025
 *      Author: anurag
 */

//ADBMS6830B datasheet Pg-53  https://www.analog.com/media/en/technical-documentation/data-sheets/adbms6830b.pdf

#include <stdint.h>


#ifndef INC_CRC_H_
#define INC_CRC_H_


#define CRC10_WIDTH		10
#define CRC10_MASK 		0x3FFu	//10 bit mask (001111111111)
#define CRC10_SEED		0x0010u //initial PEC value (0000010000)
#define CRC10_POLY_MASK 0x008Fu	//polynomial mask bit for x^10+x^7+x^3+x^2+x^1+x^0  (0010001111)


#define CRC15_WIDTH		15
#define CRC15_MASK 		0x7FFFu	//15 bit mask (0111111111111111)
#define CRC15_SEED		0x0010u //initial PEC value (000000000010000)
#define CRC15_POLY_MASK 0x4599u //polynomial mask bit for x^15+x^14+x^10+x^8+x^7+x^4+x^3+x^1 (0100010110011001)\


//
// data to CRC
// size in byte of data
// PEC register to return the data
void COMMAND_PEC(const uint8_t *data, uint16_t size, uint8_t *pec); // command to CRC, bytes of command, PEC array to return
void DATA_PEC(const uint8_t *data, uint16_t size, uint8_t *pec);   // data to CRC, bytes of data, PEC array to return


#endif /* INC_CRC_H_ */
