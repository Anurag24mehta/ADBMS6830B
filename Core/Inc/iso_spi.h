/*
 * iso_spi.h
 *
 *  Created on: Dec 13, 2025
 *      Author: anura
 */

#include <stdint.h>
#include "main.h"

#ifndef INC_ISO_SPI_H_
#define INC_ISO_SPI_H_


void BMS_WRITE(uint8_t segment, const uint8_t *CMD, uint8_t *data);
void BMS_READ(uint8_t segment, const uint8_t *CMD, uint8_t *data, uint16_t size);
void BMS_SNAP();
void BMS_UNSNAP();


#endif /* INC_ISO_SPI_H_ */
