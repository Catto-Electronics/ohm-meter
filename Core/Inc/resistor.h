#include "stm32u0xx_hal.h"

uint16_t GET_ADC_IN4(void);
double R_value(void);
double resistor_error(double raw_value);
double resistor_match(double raw_value);

void band_code(int band_number);

void flush();

void deca(void);

