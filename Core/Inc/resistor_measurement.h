/*
 * resistor_measurement.h
 *
 *  Created on: Jun 19, 2024
 *      Author: Rafael Reyes
 */

#ifndef INC_RESISTOR_MEASUREMENT_H_
#define INC_RESISTOR_MEASUREMENT_H_

#include "stm32u0xx_hal.h"

void double R_value(void);
void double resistor_error(double raw_value);
void double resistor_match(double raw_value);
void band_code(int band_number);
void flush();

#endif /* INC_RESISTOR_MEASUREMENT_H_ */
