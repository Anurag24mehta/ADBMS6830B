/*
 * bms.h
 *
 *  Created on: Dec 12, 2025
 *      Author: anurag
 */
#include <stdint.h>
#include "main.h"

#ifndef INC_BMS_H_
#define INC_BMS_H_



#define read_size (8 * SEGMENT)
#define OV_LIMIT 4.1
#define UV_LIMIT 2.8
#define OT_LIMIT 60.0
#define UT_LIMIT 0.0

typedef struct {
    float voltage;
    float temperature;

    uint8_t ov       : 1;   // Over-voltage
    uint8_t uv       : 1;   // Under-voltage
    uint8_t ot       : 1;   // Over-temperature
    uint8_t ut       : 1;   // Under-temperature
    uint8_t ow_cell  : 1;   // Cell open-wire
    uint8_t ow_temp  : 1;   // Temp sensor open-wire
    uint8_t reserved : 2;
} Cell_t;


typedef struct {
    Cell_t CELL[14];
} BMS_t;


extern BMS_t BMS[SEGMENT];
extern uint8_t WRCFGA_DATA[6];
extern uint8_t WRCFGB_DATA[6];
extern uint8_t ADCV[4];


void BMS_INIT();
void READ_ALL_CELL_VOLTAGE(BMS_t *BMS);
void PRINT_ALL_CELL_VOLTAGE(BMS_t *BMS);
void OPEN_WIRE_CHECK(BMS_t *BMS);
void Cell_SetVoltage(Cell_t *cell, int16_t v);
void Cell_SetTemp(Cell_t *cell, int16_t t);
void Cell_SetOWV(Cell_t *cell, int16_t v, uint8_t *ow_flag);
void OPEN_WIRE_TEMP(BMS_t *BMS);
void Cell_SetOWT(Cell_t *cell, int16_t t, uint8_t *ow_flag);



#endif /* INC_BMS_H_ */
