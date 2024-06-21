#ifndef RESISTOR_INCLUDED
#define RESISTOR_INCLUDED


#include "stm32u0xx_hal.h"


typedef struct R_paramTypeDef
{
  double decade;
  double Eseries;

  uint16_t r_ADC;

  double r_measured;
  double r_standard;
  double r_percentage;

} R_paramTypeDef;


void resistor_value(void);
void resistor_error(void);

void resistor_match(void);

void resistor_decade(void);

void resistor_band(int band_number);

void resistor_flush();


#endif
