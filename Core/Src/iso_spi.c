/*
 * iso_spi.c
 *
 *  Created on: Dec 13, 2025
 *      Author: anura
 */


#include "iso_spi.h"
#include "main.h"
#include "crc.h"

extern SPI_HandleTypeDef hspi2;

void BMS_WRITE(const uint8_t *CMD, uint8_t *data, uint8_t size){
	uint8_t PEC[2] = {0};
	uint8_t DPEC[2] = {0};
	COMMAND_PEC(CMD, 2, PEC);   //generating command PEC
	DATA_PEC(data, size, DPEC); //generating data PEC
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, CMD, 2, 50);
	HAL_SPI_Transmit(&hspi2, PEC, 2, 50);
	HAL_SPI_Transmit(&hspi2, data, size, 50);
	HAL_SPI_Transmit(&hspi2, DPEC, 2, 50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
}


void BMS_READ(const uint8_t *CMD, uint8_t *data, uint8_t size){
	uint8_t PEC[2] = {0} ;
	COMMAND_PEC(CMD, 2u, PEC);   //generating command PEC
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, CMD, 2, 50);
	HAL_SPI_Transmit(&hspi2, PEC, 2, 50);
	HAL_SPI_Receive(&hspi2, data, size, 50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}
