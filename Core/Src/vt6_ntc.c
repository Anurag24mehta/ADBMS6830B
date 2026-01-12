/*
 * vt6_ntc.c
 *
 *  Created on: Dec 16, 2025
 *      Author: anurag
 *
 */
#include <stdint.h>
#include <stdio.h>
#define TABLE_SIZE 33

static const float temp_table[TABLE_SIZE] = {
    -40, -35, -30, -25, -20, -15, -10, -5, 0,
      5,  10,  15,  20,  25,  30,  35,  40,
     45,  50,  55,  60,  65,  70,  75,  80,
     85,  90,  95, 100, 105, 110, 115, 120
};

static const float volt_table[TABLE_SIZE] = {
    2.44, 2.42, 2.40, 2.38, 2.35, 2.32, 2.27, 2.23, 2.17,
    2.11, 2.05, 1.99, 1.92, 1.86, 1.80, 1.74, 1.68,
    1.63, 1.59, 1.55, 1.51, 1.48, 1.45, 1.43, 1.40,
    1.38, 1.37, 1.35, 1.34, 1.33, 1.32, 1.31, 1.30
};


float voltage_to_temperature(int16_t input)
{
	float v = input * 0.000150f + 1.5f;
    if (v >= volt_table[0])
        return temp_table[0];

    if (v <= volt_table[TABLE_SIZE - 1])
        return temp_table[TABLE_SIZE - 1];

    for (uint8_t i = 0; i < TABLE_SIZE - 1; i++)
    {
        float v1 = volt_table[i];
        float v2 = volt_table[i + 1];

        if ((v <= v1) && (v >= v2))
        {
            float t1 = temp_table[i];
            float t2 = temp_table[i + 1];

            /* Linear interpolation */
            return t1 + (v - v1) * (t2 - t1) / (v2 - v1);
        }
    }

    return 0.0f;
}


