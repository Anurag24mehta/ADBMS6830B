/*
 * bms.h
 *
 *  Created on: Dec 12, 2025
 *      Author: anurag
 */
#include <stdint.h>


#ifndef INC_BMS_H_
#define INC_BMS_H_



#define SEGMENT 1			//No of Segment
#define read_size (32 * SEGMENT + 2)


typedef struct {
	float CELL[16];
} BMS_t;

extern BMS_t BMS[SEGMENT];
extern uint8_t WRCFGA_DATA[6];
extern uint8_t WRCFGB_DATA[6];
extern uint8_t ADCV[4];


void BMS_INIT();
void READ_ALL_CELL_VOLTAGE(BMS_t *BMS);
void PRINT_ALL_CELL_VOLTAGE(BMS_t *BMS);
void OPEN_WIRE_CHECK();


#endif /* INC_BMS_H_ */
