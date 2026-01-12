/*
 * iso_spi.c
 *
 *  Created on: Dec 13, 2025
 *      Author: anura
 */


#include "iso_spi.h"
#include "main.h"
#include "crc.h"
#include "command.h"
extern SPI_HandleTypeDef hspi2;

void BMS_WRITE(uint8_t segment, const uint8_t *CMD, uint8_t *data){
	uint8_t PEC[2] = {0};
	uint8_t DPEC[2] = {0};
	COMMAND_PEC(CMD, 2, PEC);   //generating command PEC
	DATA_PEC(data, 6u, DPEC); //generating data PEC
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, CMD, 2, 50);
	HAL_SPI_Transmit(&hspi2, PEC, 2, 50);
	for (uint8_t i = 1; i <= segment; i++){
		HAL_SPI_Transmit(&hspi2, data, 6u, 50);
		HAL_SPI_Transmit(&hspi2, DPEC, 2, 50);
	};
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(2);
}


void BMS_READ(uint8_t segment, const uint8_t *CMD, uint8_t *data, uint16_t size){
	uint8_t PEC[2] = {0};
	COMMAND_PEC(CMD, 2u, PEC);   //generating command PEC
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, CMD, 2, 50);
	HAL_SPI_Transmit(&hspi2, PEC, 2, 50);
	HAL_SPI_Receive(&hspi2, data, size, 50);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_Delay(2);
}

void BMS_SNAP(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, SNAP, 4, 50);			//Freeze ALL Register
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(2);
}

void BMS_UNSNAP(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, UNSNAP, 4, 50); 		//Unfreeze ALL Register
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(2);
}
