#ifndef RESISTOR_INCLUDED
#define RESISTOR_INCLUDED


#include "stm32u0xx_hal.h"


typedef struct R_paramTypeDef
{

  double decade;
  double Eseries;

  double r_measured;
  double r_standard;
  double r_percentage;

  char* color_bands[14];

} R_paramTypeDef;


uint16_t GET_ADC_IN4(void);
double resistor_value(double decade);
void resistor_error(void);

void resistor_match(void);

void resistor_decade(void);

void resistor_parse(void);

void resistor_band(uint8_t band_number, uint8_t band_value);

void resistor_measure(void);

void resistor_flush(void);


#endif
