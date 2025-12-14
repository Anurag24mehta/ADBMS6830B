/*
 * cell.c
 *
 *  Created on: Dec 12, 2025
 *      Author: anurag
 */

#include "bms.h"
#include "iso_spi.h"
#include "command.h"
#include "main.h"
#include "stdio.h"

BMS_t BMS[1];

extern SPI_HandleTypeDef hspi2;

uint8_t WRCFGA_DATA[6] = {0x84u, 0x0u, 0x0u, 0xFFu, 0x3u, 0x0u};
//WRCFGB to set Undervoltage and overvoltage threshold currently set for UV = 2.8V OV=4.2V
uint8_t WRCFGB_DATA[6] = {0x1Du, 0x52u, 0x46u, 0x0u, 0x0u, 0x0u};

uint8_t ADCV[4] = {0x2u, 0xE0u, 0x38u, 0x06u};	//RD=0, CONT=1, DCP=0, RSTF=0, OW[1:0]=00

uint8_t ADSV[4] = {0x1u, 0xE8u, 0xC3u, 0xEEu};


void BMS_INIT(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);		//wake up sequence
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
	BMS_WRITE(WRCFGA, WRCFGA_DATA, 6u);
	HAL_Delay(1);
	BMS_WRITE(WRCFGB, WRCFGB_DATA, 6u);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADCV, 4, 50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADSV, 4, 50);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);



}

void READ_ALL_CELL_VOLTAGE(BMS_t *BMS){
	uint8_t RDACALL_DATA[read_size] = {0};
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, SNAP, 4, 50);		//Freeze ALL Register
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
	BMS_READ(RDACALL, RDACALL_DATA, read_size);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, UNSNAP, 4, 50); 		//Unfreeze ALL Register
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);

	for (uint8_t module = 0; module < SEGMENT; module++){
		for (uint8_t cell = 0; cell < 16; cell++){
			uint16_t base = (module)*34;
			uint16_t i = base + cell * 2;
			int16_t raw = (int16_t)(((uint16_t)RDACALL_DATA[i+1] << 8) | RDACALL_DATA[i]);
			float voltage = raw * 0.000150f + 1.5f;
			BMS[module].CELL[cell] = voltage;
		}
	}
}

void PRINT_ALL_CELL_VOLTAGE(BMS_t *BMS){
    for (uint8_t module = 0; module < SEGMENT; module++){
        printf("=== Module %d ===\r\n", module);
        for (uint8_t cell = 0; cell < 16; cell++){
            printf("Cell[%d] = %.3f V\r\n", cell, BMS[module].CELL[cell]);
        }
    }
}

void OPEN_WIRE_CHECK(){
	uint8_t ADSV_EVEN[4] = {0x1u, 0xE9u, 0x48u, 0xDCu};
	uint8_t ADSV_ODD[4] = {0x1u, 0xEAu, 0x5Eu, 0xB8u};
	uint8_t RDSALL_DATA[read_size] = {0};
	uint8_t OPEN_WIRE_FLAG = 0;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADSV_ODD, 4, 50);		//Check for odd cell for open wire
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, SNAP, 4, 50);			//Freeze ALL Register
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);

	BMS_READ(RDSALL, RDSALL_DATA, read_size);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, UNSNAP, 4, 50); 		//Unfreeze ALL Register
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);

	for (uint8_t module = 0; module < SEGMENT; module++){
		for (uint8_t cell = 0; cell < 16; cell = cell + 2){
			uint16_t base = (module)*34;
			uint16_t i = base + cell * 2;
			int16_t raw = (int16_t)(((uint16_t)RDSALL_DATA[i+1] << 8) | RDSALL_DATA[i]);
			float voltage = raw * 0.000150f + 1.5f;
			if (voltage < 0.1) {
				OPEN_WIRE_FLAG = 1;
				printf("Module[%d] Cell[%d] is open \r\n", module, cell);
			}
		}
	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);		//wake up sequence
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADSV_EVEN, 4, 50);		//Check for even cell for open wire
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, SNAP, 4, 50);			//Freeze ALL Register
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);

	BMS_READ(RDSALL, RDSALL_DATA, read_size);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, UNSNAP, 4, 50); 		//Unfreeze ALL Register
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
	HAL_Delay(1);
//
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi2, ADSV_INIT, 4, 50); 		//Close S ADC
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10,GPIO_PIN_SET);
//


	for (uint8_t module = 0; module < SEGMENT; module++){
		for (uint8_t cell = 1; cell < 16; cell = cell + 2){
			uint16_t base = (module)*34;
			uint16_t i = base + cell * 2;
			int16_t raw = (int16_t)(((uint16_t)RDSALL_DATA[i+1] << 8) | RDSALL_DATA[i]);
			float voltage = raw * 0.000150f + 1.5f;
			if (voltage < 0.1) {
				OPEN_WIRE_FLAG = 1;
				printf("Module[%d] Cell[%d] is open \r\n", module, cell);
			}
		}
	}

	if (OPEN_WIRE_FLAG == 1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_RESET);

	}


}

