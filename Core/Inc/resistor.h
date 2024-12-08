#ifndef RESISTOR_INCLUDED
#define RESISTOR_INCLUDED


#include "stm32u0xx_hal.h"
#include "stm32u083xx.h"


typedef struct R_paramTypeDef
{

  double decade;
  double Eseries;

  uint32_t ADC;

  double r_measured;
  double r_standard;
  double r_percentage;

  uint8_t flags;

  char color_bands[14];
  char band4[21];
  char band5[21];
  uint8_t color_index;

} R_paramTypeDef;


double resistor_value(double decade);
void resistor_error(void);
void resistor_match(void);
double r_standard(double index);
void resistor_decade(void);
void resistor_parse(void);
void resistor_band(uint32_t band_number, uint32_t band_value);
void resistor_measure(void);
void resistor_flush(void);


#endif
