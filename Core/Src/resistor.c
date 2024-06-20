/*
 * resistore_measurement.c
 *
 *  Created on: Jun 19, 2024
 *      Author: Rafael Reyes
 */

#include "resistor.h"
#include "math.h"

extern ADC_HandleTypeDef hadc1;


double decade = 0;
double raw_value = 0;
int r_standard = 0;
double r_percentage = 0;
double Eseries = 48;


uint16_t GET_ADC_IN4(void)
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);

	uint16_t i = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_Stop(&hadc1);

	return i;
}

double R_value(void)
{
  double r_value;
  int raw = GET_ADC_IN4();
  double voltage_value;

  voltage_value = ((double)raw/1023); // percentage of total voltage

  r_value = ((decade*5.1)/voltage_value)-(decade*5.1);

  return r_value;
}

void deca(void)
{
	decade = 100;
}

double resistor_error(double raw_value)
{
  double error_percentage;
  double std_value = resistor_match(raw_value);

  error_percentage = ((raw_value - std_value) / std_value) * 100;

  r_percentage = error_percentage;


  return error_percentage;
}

double resistor_match(double raw_value)
{
  double r_value;
  double r_index;

  r_index = round(Eseries*(log10(raw_value/decade)));

  r_value = round(decade*pow(10, r_index/Eseries)/(decade/10));
  r_value *= decade/10;

  if(Eseries == 24 && (int)r_index%24 >= 11 && (int)r_index%24 <= 17)
  {
  	r_value += 1*decade/10;
  }

  r_standard = r_value;

  return r_value;
}

void color_code(int band_number)
{

}

void flush()
{
  raw_value = 0;
  r_standard = 0;
  r_percentage = 0;
}
