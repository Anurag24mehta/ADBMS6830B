/*
 * command.c
 *
 *  Created on: Dec 11, 2025
 *      Author: anurag
 */
#include "command.h"

const uint8_t WRCFGA[2] = {0x0u, 0x01u};
const uint8_t WRCFGB[2] = {0x0u, 0x24u};
const uint8_t RDCFGA[2] = {0x0u, 0x02u};
const uint8_t RDCFGB[2] = {0x0u, 0x26u};
const uint8_t RDACA[2] = {0x0u, 0x44u};
const uint8_t RDACB[2] = {0x0u, 0x46u};
const uint8_t RDACC[2] = {0x0u, 0x48u};
const uint8_t RDACD[2] = {0x0u, 0x4Au};
const uint8_t RDACE[2] = {0x0u, 0x49u};
const uint8_t RDACF[2] = {0x0u, 0x4Bu};
const uint8_t RDACALL[2] = {0x0u, 0x4Cu};
const uint8_t SNAP[4] = {0x0u, 0x2D, 0xD2, 0xA2};
const uint8_t UNSNAP[4] = {0x0u, 0x2F, 0x4F, 0xF4};
const uint8_t RDSTATD[2] = {0x0u, 0x33u};
const uint8_t RDSALL[2] = {0x0u, 0x10u};





