/*
 * iso_spi.h
 *
 *  Created on: Dec 13, 2025
 *      Author: anura
 */

#include <stdint.h>

#ifndef INC_ISO_SPI_H_
#define INC_ISO_SPI_H_


void BMS_WRITE(const uint8_t *CMD, uint8_t *data, uint8_t size);
void BMS_READ(const uint8_t *CMD, uint8_t *data, uint8_t size);



#endif /* INC_ISO_SPI_H_ */
