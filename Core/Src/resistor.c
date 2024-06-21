/*
 * resistor.c
 *
 *  Created on: Jun 19, 2024
 *      Author: Rafael Reyes
 */

#include "resistor.h"
#include "math.h"


extern R_paramTypeDef R_config;
extern R_paramTypeDef empty;    // Reference for flushing values from previous iteration


void resisor_value(void)        // Reads the ADC output and converts it into the value of the measured resistor in ohms
{

	double raw = R_config.r_ADC;
	double decade = R_config.decade;
	double voltage_value;

	voltage_value = ((double)raw/4096); // percentage of total voltage

	R_config.r_measured = ((decade*5.1)/voltage_value)-(decade*5.1);

}


void resistor_error(void)       // Calculates the percent error of the measured resistor value with the closest standard value in the series
{

	double error_percentage;
	double measured = R_config.r_measured;
	double std_value = R_config.r_standard;

	error_percentage = ((measured - std_value) / std_value) * 100;

	R_config.r_percentage = error_percentage;

}


void resistor_match(void)       // Finds the nearest standard resistor value from the measured resistor
{

	double Eseries = R_config.Eseries;
	double decade = R_config.decade;
	double measured = R_config.r_measured;
	double std_value;
	double r_index;

	r_index = round(Eseries*(log10(measured/decade)));

	std_value = round(decade*pow(10, r_index/Eseries)/(decade/10));
	std_value *= decade/10;

	if(Eseries == 24 && (int)r_index%24 >= 11 && (int)r_index%24 <= 17)
	{
		std_value += 1*decade/10;
	}

	R_config.r_standard = std_value;

}


void resistor_decade(void)
{



}


void resistor_band(int band_number) // Determines the color bands of the measured resistor
{



}


void resistor_flush()
{

	R_config.r_ADC = 0;
	R_config.r_measured = 0;
	R_config.r_percentage = 0;
	R_config.r_standard = 0;

}
