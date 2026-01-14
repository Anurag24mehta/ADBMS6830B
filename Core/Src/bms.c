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
#include <stdio.h>
#include "vt6_ntc.h"
#include "print.h"


extern SPI_HandleTypeDef hspi2;

void BMS_INIT(){
	uint8_t WRCFGA_DATA[6] = {0x84u, 0x0u, 0x0u, 0xFFu, 0x3u, 0x0u};
	//WRCFGB to set Undervoltage and overvoltage threshold currently set for UV = 2.8V OV=4.2V
	uint8_t WRCFGB_DATA[6] = {0x1Du, 0x52u, 0x46u, 0x0u, 0x0u, 0x0u};
	uint8_t ADCV[4] = {0x2u, 0xE0u, 0x38u, 0x06u};	//RD=0, CONT=1, DCP=0, RSTF=0, OW[1:0]=00
	uint8_t ADSV[4] = {0x1u, 0xE8u, 0xC3u, 0xEEu};  //RD=0, CONT=0, DCP=0, OW[1:0] = 00
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);		//wake up sequence
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(1);
	BMS_WRITE(SEGMENT, WRCFGA, WRCFGA_DATA);
	BMS_WRITE(SEGMENT, WRCFGB, WRCFGB_DATA);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADCV, 4, 50);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(2);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
//	HAL_SPI_Transmit(&hspi2, ADSV, 4, 50);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);



}

void READ_ALL_CELL_VOLTAGE(BMS_t *BMS){

	uint8_t RDACA_DATA[SEGMENT*8] = {0};
	uint8_t RDACB_DATA[SEGMENT*8] = {0};
	uint8_t RDACC_DATA[SEGMENT*8] = {0};
	uint8_t RDACD_DATA[SEGMENT*8] = {0};
	uint8_t RDACE_DATA[SEGMENT*8] = {0};


	BMS_SNAP();
	BMS_READ(SEGMENT, RDACA, RDACA_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDACB, RDACB_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDACC, RDACC_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDACD, RDACD_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDACE, RDACE_DATA, SEGMENT*8u);
	BMS_UNSNAP();

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetVoltage(&BMS[module].CELL[0], (int16_t)(((uint16_t)RDACA_DATA[i+1] << 8) | RDACA_DATA[i]));
		Cell_SetVoltage(&BMS[module].CELL[1], (int16_t)(((uint16_t)RDACA_DATA[i+3] << 8) | RDACA_DATA[i+2]));
		Cell_SetVoltage(&BMS[module].CELL[2], (int16_t)(((uint16_t)RDACA_DATA[i+5] << 8) | RDACA_DATA[i+4]));

	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetVoltage(&BMS[module].CELL[3], (int16_t)(((uint16_t)RDACB_DATA[i+1] << 8) | RDACB_DATA[i]));
			Cell_SetVoltage(&BMS[module].CELL[4], (int16_t)(((uint16_t)RDACB_DATA[i+3] << 8) | RDACB_DATA[i+2]));
			Cell_SetVoltage(&BMS[module].CELL[5], (int16_t)(((uint16_t)RDACB_DATA[i+5] << 8) | RDACB_DATA[i+4]));
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetVoltage(&BMS[module].CELL[6], (int16_t)(((uint16_t)RDACC_DATA[i+1] << 8) | RDACC_DATA[i]));
			Cell_SetVoltage(&BMS[module].CELL[7], (int16_t)(((uint16_t)RDACC_DATA[i+3] << 8) | RDACC_DATA[i+2]));
			Cell_SetVoltage(&BMS[module].CELL[8], (int16_t)(((uint16_t)RDACC_DATA[i+5] << 8) | RDACC_DATA[i+4]));
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetVoltage(&BMS[module].CELL[9], (int16_t)(((uint16_t)RDACD_DATA[i+1] << 8) | RDACD_DATA[i]));
			Cell_SetVoltage(&BMS[module].CELL[10], (int16_t)(((uint16_t)RDACD_DATA[i+3] << 8) | RDACD_DATA[i+2]));
			Cell_SetVoltage(&BMS[module].CELL[11], (int16_t)(((uint16_t)RDACD_DATA[i+5] << 8) | RDACD_DATA[i+4]));
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetVoltage(&BMS[module].CELL[12], (int16_t)(((uint16_t)RDACE_DATA[i+1] << 8) | RDACE_DATA[i]));
			Cell_SetVoltage(&BMS[module].CELL[13], (int16_t)(((uint16_t)RDACE_DATA[i+3] << 8) | RDACE_DATA[i+2]));
	}
}

void PRINT_ALL_CELL_VOLTAGE(BMS_t *BMS){
    for (uint8_t module = 0; module < SEGMENT; module++){
        printf("========================== Segment %d ===========================\r\n", module+1);
        printf("+------+-----------+-------------+----+----+-----+----+----+-----+\r\n");
        printf("| Cell | Voltage V | Temp (degC) | OV | UV | OWC | OT | UT | OWT |\r\n");
        printf("+------+-----------+-------------+----+----+-----+----+----+-----+\r\n");
        for (uint8_t cell = 0; cell < 14; cell++){
            printf("| %4d | %9.3f | %11.3f | %2d | %2d | %2d | %2d  | %2d | %2d |\r\n",
            		cell + 1,
					BMS[module].CELL[cell].voltage,
					BMS[module].CELL[cell].temperature,
					BMS[module].CELL[cell].ov,
					BMS[module].CELL[cell].uv,
					BMS[module].CELL[cell].ow_cell,
					BMS[module].CELL[cell].ot,
					BMS[module].CELL[cell].ut,
					BMS[module].CELL[cell].ow_temp);
        }
        printf("+------+-----------+-------------+----+----+-----+----+----+-----+\r\n");
    }
}

void OPEN_WIRE_CHECK(BMS_t *BMS){
	uint8_t ADSV_ODD[4] = {0x1u, 0xE9u, 0x48u, 0xDCu};
	uint8_t ADSV_EVEN[4] = {0x1u, 0xEAu, 0x5Eu, 0xB8u};
	uint8_t OPEN_WIRE_FLAG = 0;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADSV_ODD, 4, 50);		//Check for odd cell for open wire
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
//	HAL_Delay(8);
	uint8_t RDACA_DATA[SEGMENT*8] = {0};
	uint8_t RDACB_DATA[SEGMENT*8] = {0};
	uint8_t RDACC_DATA[SEGMENT*8] = {0};
	uint8_t RDACD_DATA[SEGMENT*8] = {0};
	uint8_t RDACE_DATA[SEGMENT*8] = {0};

	BMS_SNAP();
	BMS_READ(SEGMENT, RDSVA, RDACA_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVB, RDACB_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVC, RDACC_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVD, RDACD_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVE, RDACE_DATA, SEGMENT*8u);
	BMS_UNSNAP();

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetOWV(&BMS[module].CELL[0], (int16_t)(((uint16_t)RDACA_DATA[i+1] << 8) | RDACA_DATA[i]), &OPEN_WIRE_FLAG);
		Cell_SetOWV(&BMS[module].CELL[2], (int16_t)(((uint16_t)RDACA_DATA[i+5] << 8) | RDACA_DATA[i+4]), &OPEN_WIRE_FLAG);

	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[4], (int16_t)(((uint16_t)RDACB_DATA[i+3] << 8) | RDACB_DATA[i+2]), &OPEN_WIRE_FLAG);
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[6], (int16_t)(((uint16_t)RDACC_DATA[i+1] << 8) | RDACC_DATA[i]), &OPEN_WIRE_FLAG);
			Cell_SetOWV(&BMS[module].CELL[8], (int16_t)(((uint16_t)RDACC_DATA[i+5] << 8) | RDACC_DATA[i+4]), &OPEN_WIRE_FLAG);
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[10], (int16_t)(((uint16_t)RDACD_DATA[i+3] << 8) | RDACD_DATA[i+2]), &OPEN_WIRE_FLAG);
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[12], (int16_t)(((uint16_t)RDACE_DATA[i+1] << 8) | RDACE_DATA[i]), &OPEN_WIRE_FLAG);
	}

	BMS_INIT();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADSV_EVEN, 4, 50);		//Check for odd cell for open wire
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	BMS_SNAP();
	BMS_READ(SEGMENT, RDSVA, RDACA_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVB, RDACB_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVC, RDACC_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVD, RDACD_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDSVE, RDACE_DATA, SEGMENT*8u);
	BMS_UNSNAP();

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetOWV(&BMS[module].CELL[1], (int16_t)(((uint16_t)RDACA_DATA[i+3] << 8) | RDACA_DATA[i+2]), &OPEN_WIRE_FLAG);

	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[3], (int16_t)(((uint16_t)RDACB_DATA[i+1] << 8) | RDACB_DATA[i]), &OPEN_WIRE_FLAG);
			Cell_SetOWV(&BMS[module].CELL[5], (int16_t)(((uint16_t)RDACB_DATA[i+5] << 8) | RDACB_DATA[i+4]), &OPEN_WIRE_FLAG);
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[7], (int16_t)(((uint16_t)RDACC_DATA[i+3] << 8) | RDACC_DATA[i+2]), &OPEN_WIRE_FLAG);
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[9], (int16_t)(((uint16_t)RDACD_DATA[i+1] << 8) | RDACD_DATA[i]), &OPEN_WIRE_FLAG);
			Cell_SetOWV(&BMS[module].CELL[11], (int16_t)(((uint16_t)RDACD_DATA[i+5] << 8) | RDACD_DATA[i+4]), &OPEN_WIRE_FLAG);
	}
	for (uint8_t module = 0; module < SEGMENT; module++){
			uint16_t i = module * 8;
			Cell_SetOWV(&BMS[module].CELL[13], (int16_t)(((uint16_t)RDACE_DATA[i+3] << 8) | RDACE_DATA[i+2]), &OPEN_WIRE_FLAG);
	}
	if (OPEN_WIRE_FLAG == 1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
	}
}


void READ_ALL_TEMP(BMS_t *BMS){
	uint8_t WRCFGA_SEL[6] = {0x81u, 0x0u, 0x0u, 0xFFu, 0x1u, 0x0u};  //To Change The Select Lines
	uint8_t ADAX[4] = {0x4u, 0x10u, 0x51u, 0x14u};  //Read All GPIO //Select Line = 1 Is Selected
	uint8_t RDAUXA_DATA[SEGMENT*8] = {0};
	uint8_t RDAUXB_DATA[SEGMENT*8] = {0};
	uint8_t RDAUXC_DATA[SEGMENT*8] = {0};

	BMS_WRITE(SEGMENT, WRCFGA, WRCFGA_SEL);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADAX, 4, 50);		//START ADAX Conversion
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(8);
	BMS_SNAP();
	BMS_READ(SEGMENT, RDAUXA, RDAUXA_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDAUXB, RDAUXB_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDAUXC, RDAUXC_DATA, SEGMENT*8u);
	BMS_UNSNAP();


	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetTemp(&BMS[module].CELL[0], (int16_t)(((uint16_t)RDAUXA_DATA[i+1] << 8) | RDAUXA_DATA[i]));
		Cell_SetTemp(&BMS[module].CELL[1], (int16_t)(((uint16_t)RDAUXA_DATA[i+3] << 8) | RDAUXA_DATA[i+2]));
		Cell_SetTemp(&BMS[module].CELL[2], (int16_t)(((uint16_t)RDAUXA_DATA[i+5] << 8) | RDAUXA_DATA[i+4]));
	}

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetTemp(&BMS[module].CELL[3], (int16_t)(((uint16_t)RDAUXB_DATA[i+1] << 8) | RDAUXB_DATA[i]));
		Cell_SetTemp(&BMS[module].CELL[4], (int16_t)(((uint16_t)RDAUXB_DATA[i+3] << 8) | RDAUXB_DATA[i+2]));
		Cell_SetTemp(&BMS[module].CELL[5], (int16_t)(((uint16_t)RDAUXB_DATA[i+5] << 8) | RDAUXB_DATA[i+4]));
	}

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetTemp(&BMS[module].CELL[7], (int16_t)(((uint16_t)RDAUXC_DATA[i+1] << 8) | RDAUXC_DATA[i]));
		Cell_SetTemp(&BMS[module].CELL[9], (int16_t)(((uint16_t)RDAUXC_DATA[i+3] << 8) | RDAUXC_DATA[i+2]));
		Cell_SetTemp(&BMS[module].CELL[11], (int16_t)(((uint16_t)RDAUXC_DATA[i+5] << 8) | RDAUXC_DATA[i+4]));
	}

	BMS_INIT();
//	BMS_WRITE(SEGMENT, WRCFGA, WRCFGA_SEL);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADAX, 4, 50);		//START ADAX Conversion
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(8);
	BMS_SNAP();
	BMS_READ(SEGMENT, RDAUXB, RDAUXB_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDAUXC, RDAUXC_DATA, SEGMENT*8u);
	BMS_UNSNAP();

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetTemp(&BMS[module].CELL[6], (int16_t)(((uint16_t)RDAUXB_DATA[i+5] << 8) | RDAUXB_DATA[i+4]));
	}

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetTemp(&BMS[module].CELL[8], (int16_t)(((uint16_t)RDAUXC_DATA[i+1] << 8) | RDAUXC_DATA[i]));
		Cell_SetTemp(&BMS[module].CELL[10], (int16_t)(((uint16_t)RDAUXC_DATA[i+3] << 8) | RDAUXC_DATA[i+2]));
		Cell_SetTemp(&BMS[module].CELL[12], (int16_t)(((uint16_t)RDAUXC_DATA[i+5] << 8) | RDAUXC_DATA[i+4]));
		Cell_SetTemp(&BMS[module].CELL[13], (int16_t)(((uint16_t)RDAUXC_DATA[i+5] << 8) | RDAUXC_DATA[i+4]));
	}

}

void OPEN_WIRE_TEMP(BMS_t *BMS){
	uint8_t OPEN_WIRE_FLAG = 0;
	uint8_t WRCFGA_SEL[6] = {0x81u, 0x0u, 0x0u, 0xFFu, 0x1u, 0x0u};  //To Change The Select Lines
	uint8_t ADAX[4] = {0x05u, 0x90u, 0x9Du, 0x7Eu};
	uint8_t RDAUXA_DATA[SEGMENT*8] = {0};
	uint8_t RDAUXB_DATA[SEGMENT*8] = {0};
	uint8_t RDAUXC_DATA[SEGMENT*8] = {0};

	BMS_WRITE(SEGMENT, WRCFGA, WRCFGA_SEL);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADAX, 4, 50);		//START ADAX Conversion
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(8);
	BMS_SNAP();
	BMS_READ(SEGMENT, RDAUXA, RDAUXA_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDAUXB, RDAUXB_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDAUXC, RDAUXC_DATA, SEGMENT*8u);
	BMS_UNSNAP();


	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetOWT(&BMS[module].CELL[0], (int16_t)(((uint16_t)RDAUXA_DATA[i+1] << 8) | RDAUXA_DATA[i]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[1], (int16_t)(((uint16_t)RDAUXA_DATA[i+3] << 8) | RDAUXA_DATA[i+2]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[2], (int16_t)(((uint16_t)RDAUXA_DATA[i+5] << 8) | RDAUXA_DATA[i+4]), &OPEN_WIRE_FLAG);
	}

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetOWT(&BMS[module].CELL[3], (int16_t)(((uint16_t)RDAUXB_DATA[i+1] << 8) | RDAUXB_DATA[i]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[4], (int16_t)(((uint16_t)RDAUXB_DATA[i+3] << 8) | RDAUXB_DATA[i+2]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[5], (int16_t)(((uint16_t)RDAUXB_DATA[i+5] << 8) | RDAUXB_DATA[i+4]), &OPEN_WIRE_FLAG);
	}

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetOWT(&BMS[module].CELL[7], (int16_t)(((uint16_t)RDAUXC_DATA[i+1] << 8) | RDAUXC_DATA[i]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[9], (int16_t)(((uint16_t)RDAUXC_DATA[i+3] << 8) | RDAUXC_DATA[i+2]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[11], (int16_t)(((uint16_t)RDAUXC_DATA[i+5] << 8) | RDAUXC_DATA[i+4]), &OPEN_WIRE_FLAG);
	}

//	BMS_WRITE(SEGMENT, WRCFGA, WRCFGA_SEL);
	BMS_INIT();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, ADAX, 4, 50);		//START ADAX Conversion
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
	HAL_Delay(8);
	BMS_SNAP();
	BMS_READ(SEGMENT, RDAUXB, RDAUXB_DATA, SEGMENT*8u);
	BMS_READ(SEGMENT, RDAUXC, RDAUXC_DATA, SEGMENT*8u);
	BMS_UNSNAP();

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetOWT(&BMS[module].CELL[6], (int16_t)(((uint16_t)RDAUXB_DATA[i+5] << 8) | RDAUXB_DATA[i+4]), &OPEN_WIRE_FLAG);
	}

	for (uint8_t module = 0; module < SEGMENT; module++){
		uint16_t i = module * 8;
		Cell_SetOWT(&BMS[module].CELL[8], (int16_t)(((uint16_t)RDAUXC_DATA[i+1] << 8) | RDAUXC_DATA[i]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[10], (int16_t)(((uint16_t)RDAUXC_DATA[i+3] << 8) | RDAUXC_DATA[i+2]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[12], (int16_t)(((uint16_t)RDAUXC_DATA[i+5] << 8) | RDAUXC_DATA[i+4]), &OPEN_WIRE_FLAG);
		Cell_SetOWT(&BMS[module].CELL[13], (int16_t)(((uint16_t)RDAUXC_DATA[i+5] << 8) | RDAUXC_DATA[i+4]), &OPEN_WIRE_FLAG);
	}
	if (OPEN_WIRE_FLAG == 1){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
	}
}

void Cell_SetVoltage(Cell_t *cell, int16_t v){
	float x = v * 0.000150f + 1.5f;
	cell->voltage = x;
	cell->ov = 0;
	cell->uv = 0;
	cell-> ov = (x > OV_LIMIT);
	cell-> uv = (x < UV_LIMIT);
}

void Cell_SetTemp(Cell_t *cell, int16_t t){
	float temp = voltage_to_temperature(t);
	float x = t * 0.000150f + 1.5f;
	cell-> ot = 0;
	cell-> ut = 0;
	cell->temperature = temp;
	cell-> ot = (temp > OT_LIMIT);
	cell-> ut = (temp < UT_LIMIT);
}

void Cell_SetOWV(Cell_t *cell, int16_t v, uint8_t *ow_flag){
	float x = v * 0.000150f + 1.5f;
//		printf("%f \r\n",x);

	cell->ow_cell = 0;
	if (x < 0.4){
		cell->ow_cell = 1;
		*ow_flag = 1;
	}
}
void Cell_SetOWT(Cell_t *cell, int16_t t, uint8_t *ow_flag){
	float x = t * 0.000150f + 1.5f;
	cell->ow_temp = 0;
	if(x > 3.0){
		cell->ow_temp = 1;
		*ow_flag = 1;
	}
}






