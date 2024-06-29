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


extern ADC_HandleTypeDef hadc1;  // change your handler here accordingly
extern R_paramTypeDef R_config;


uint16_t GET_ADC_IN4(void);
void resistor_measure(void);



uint16_t GET_ADC_IN4(void)
{
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);

	uint16_t i = HAL_ADC_GetValue(&hadc1);

	HAL_ADC_Stop(&hadc1);

	return i;
}


/**************************************************
Brief: Reads the ADC output and converts it into the value of the measured resistor in ohms
Return: Resistance in Ohms
***************************************************/
double resisor_value(double decade)
{

	double raw = GET_ADC_IN4();
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

	double decade = 100000;

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


	//Brief: Get 4-band resistor colors
	resistor_band(1, (uint32_t)Std % (uint32_t)R_config.decade);       // First band value
	resistor_band(2, (uint32_t)Std % (uint32_t)(R_config.decade/10));  // Second band value

	resistor_band(3 ,log10(R_config.decade));      // Decade multiplier


	//Brief: Get 5-band resistor colors
	resistor_band(1, (uint32_t)Std % (uint32_t)R_config.decade);       // First band value
	resistor_band(2, (uint32_t)Std % (uint32_t)(R_config.decade/10));  // Second band value
	resistor_band(3, (uint32_t)Std % (uint32_t)(R_config.decade/100));  // Second band value

	resistor_band(4 ,log10(R_config.decade));      // Decade multiplier

}


/**************************************************
Brief: Determines the color bands of the measured resistor
Return: NONE
***************************************************/
void resistor_band(uint8_t band_number, uint8_t band_value)
{


	//Brief: Space columns numbers based on band_number and UI configuration
	uint8_t band_index = 0;




	switch(band_value)
	{
		case 0:
			R_config.color_bands[band_index] = "BK ";
			break;
		case 1:
			R_config.color_bands[band_index] = "BR ";
			break;
		case 2:
			R_config.color_bands[band_index] = "R ";
			break;
		case 3:
			R_config.color_bands[band_index] = "O ";
			break;
		case 4:
			R_config.color_bands[band_index] = "Y ";
			break;
		case 5:
			R_config.color_bands[band_index] = "G ";
			break;
		case 6:
			R_config.color_bands[band_index] = "B ";
			break;
		case 7:
			R_config.color_bands[band_index] = "V ";
			break;
		case 8:
			R_config.color_bands[band_index] = "GY ";
			break;
		case 9:
			R_config.color_bands[band_index] = "W ";
			break;
		default:
			R_config.color_bands[band_index] = "NA";
			break;
	}

}


/**************************************************
Brief: Executes all necessary functions for data acquisition and conversion
Return: NONE
***************************************************/
void resistor_measure(void)
{
	resistor_decade();
	resistor_match();
	resistor_error();
	resistor_parse();
}


/**************************************************
Brief: Resets all values in struct R_config
Return: NONE
***************************************************/
void resistor_flush(void)
{

	R_config.r_measured = 0;
	R_config.r_percentage = 0;
	R_config.r_standard = 0;

}
