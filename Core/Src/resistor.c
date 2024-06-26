/*
 * resistor.c
 *
 *  Created on: Jun 19, 2024
 *      Author: Rafael Reyes
 */

#include "resistor.h"
#include "math.h"

#define R_MUX 3.3

extern R_paramTypeDef R_config;
extern R_paramTypeDef empty;    // Reference for flushing values from previous iteration



/**************************************************
Brief: Reads the ADC output and converts it into the value of the measured resistor in ohms
Return: Resistance in Ohms
***************************************************/
uint32_t resisor_value(double decade)
{

	double raw = R_config.r_ADC;
	double decade_resistor = decade * R_MUX;
	double ADC_ratio;
	double resistor_value;

	ADC_ratio = ((double)raw/4096); // percentage of total voltage

	//R_config.r_measured = ((decade)/ADC_ratio)-(decade); // Old ADC to Resistance conversion

	resistor_value = (ADC_ratio * decade_resistor)/(1 - ADC_ratio);

	return resistor_value;
}


/**************************************************
Brief: Calculates the percent error of the measured resistor
	   value with the closest standard value in the series
Return: NONE
***************************************************/
void resistor_error(void)
{
	double error_percentage;
	double measured = R_config.r_measured;
	double std_value = R_config.r_standard;

	error_percentage = ((measured - std_value) / std_value) * 100;

	R_config.r_percentage = error_percentage;

}


/**************************************************
Brief: Finds the nearest standard resistor value from the measured resistor
Return: NONE
***************************************************/
void resistor_match(void)
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


/**************************************************
Brief: Sorts through measured values and determines the appropriate decade
Return: Resistor decade 10, 100, 1k, 10k, 100k, 1M
***************************************************/
void resistor_decade(void)
{

	for(double decade = 1000000; decade >= 1; decade /= 10)
	{
		if(resisor_value(decade) >= decade)
		{
			R_config.decade = decade;
			R_config.r_measured = resisor_value(decade);
		}
	}
}


/**************************************************
Brief: Determines the color bands of the measured resistor
Return: NONE
***************************************************/
void resistor_band(int band_number)
{



}


/**************************************************
Brief: Resets all values in struct R_config
Return: NONE
***************************************************/
void resistor_flush()
{

	R_config.r_ADC = 0;
	R_config.r_measured = 0;
	R_config.r_percentage = 0;
	R_config.r_standard = 0;

}
