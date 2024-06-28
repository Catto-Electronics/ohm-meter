/*
 * resistor.c
 *
 *  Created on: Jun 19, 2024
 *      Author: Rafael Reyes
 */

#include "stdio.h"
#include "resistor.h"
#include "i2c-lcd.h"
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

	double decade = 100;

		if((resisor_value(decade) >= decade) && (resisor_value(decade) < decade*10))
		{
			R_config.decade = decade;
			R_config.r_measured = resisor_value(decade);
		}

}

/**************************************************
Brief: Parses the standard resistor values to determine the resistor band colors
Return: NONE
***************************************************/
void resistor_parse(void)
{
	R_config.r_standard = 120;
	R_config.decade = 100;

	uint32_t Std = R_config.r_standard;

	lcd_put_cur(0, 0);

	resistor_band((uint32_t)Std % (uint32_t)R_config.decade);       // First band value
	resistor_band((uint32_t)Std % (uint32_t)(R_config.decade/10));  // Second band value
	//resistor_band(Std % (decade/100)); // Third band value

	resistor_band(log10(R_config.decade));      // Decade multiplier

}


/**************************************************
Brief: Determines the color bands of the measured resistor
Return: NONE
***************************************************/
void resistor_band(uint8_t band_value)
{

	switch(band_value)
	{
		case 0:
			lcd_send_string("BK");
			break;
		case 1:
			lcd_send_string("BR");
			break;
		case 2:
			lcd_send_string("R");
			break;
		case 3:
			lcd_send_string("O");
			break;
		case 4:
			lcd_send_string("Y");
			break;
		case 5:
			lcd_send_string("G");
			break;
		case 6:
			lcd_send_string("B");
			break;
		case 7:
			lcd_send_string("V");
			break;
		case 8:
			lcd_send_string("GY");
			break;
		case 9:
			lcd_send_string("W");
			break;
	}

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
