/*
 * resistore_measurement.c
 *
 *  Created on: Jun 19, 2024
 *      Author: Rafael Reyes
 */

#include "resistor_measurement.h"


double R_value(void)
{
  double r_value;
  int raw = analogRead(0);
  double voltage_value;


  voltage_value = ((double)raw/1023); // percentage of total voltage

  r_value = ((decade*5.1)/voltage_value)-(decade*5.1);

  Serial.println(r_value);

  r_measured = r_value;

  Serial.print(r_value);
  return r_value;
}

double resistor_error(double raw_value)
{
  double error_percentage;
  double std_value = resistor_match(raw_value);

  error_percentage = ((raw_value - std_value) / std_value) * 100;

  r_percentage = error_percentage;

  Serial.print(" ");
  Serial.println(error_percentage);

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

  Serial.print(" ");
  Serial.print(r_value);

  return r_value;
}

void color_code(int band_number)
{
  switch(band_number)
  {
    case 0: Serial.print("Black ");
     break;
    case 1: Serial.print("Brown ");
     break;
    case 2: Serial.print("Red ");
     break;
    case 3: Serial.print("Orange ");
     break;
    case 4: Serial.print("Yellow ");
     break;
    case 5: Serial.print("Green ");
     break;
    case 6: Serial.print("Blue ");
     break;
    case 7: Serial.print("Violet ");
     break;
    case 8: Serial.print("Gray ");
     break;
    case 9: Serial.print("White ");
     break;
  }
}

void flush()
{
  double r_measured, r_standard, r_percentage = NULL;
}
