#include "stdio.h"
#include "resistor.h"
#include "i2c-lcd.h"
#include "math.h"

extern R_paramTypeDef R_config;

void truncate(double resistor_value, double standard_value);


char writeString[21];
char R_Val[7];
char E_Val[5];

void screenMenu(void){
	lcd_put_cur(0, 0);
	lcd_send_string("       CATTO");
	lcd_put_cur(1, 0);
	lcd_send_string("    ELECTRONICS");
	HAL_Delay(1000);

	lcd_put_cur(0, 0);
	lcd_send_string("              ");
	lcd_put_cur(1, 0);
	lcd_send_string("     OHM  METER");
	HAL_Delay(1000);

	lcd_put_cur(0, 0);
	lcd_send_string("Select E-Series:");
	lcd_put_cur(1, 0);
	lcd_send_string("  6  12  24  48  96");
	HAL_Delay(1000);
}

void screenMeasurements(void)
{
	truncate(R_config.r_measured, R_config.r_standard);
	lcd_put_cur(0, 0);
	sprintf(writeString, "Value:%6s       ", R_Val);
	lcd_send_string(writeString);
	lcd_put_cur(1, 0);
	sprintf(writeString, "EVal:%4s  Er:%.1f%%  ", E_Val, R_config.r_percentage);
	lcd_send_string(writeString);
	lcd_put_cur(2, 0);
	lcd_send_string(R_config.band5);
	lcd_put_cur(3, 0);
	lcd_send_string(R_config.band5);
	HAL_Delay(500);
}


void truncate(double resistor_value, double standard_value)
{

	switch((int)R_config.decade)
	{
		case 1:
			sprintf(R_Val, "%.2f  ", resistor_value);
			sprintf(E_Val, "%.1f ", standard_value);
			break;
		case 10:
			sprintf(R_Val, "%.2f  ", resistor_value);
			sprintf(E_Val, "%.1f", standard_value);
			break;
		case 100:
			sprintf(R_Val, "%.2f ", resistor_value);
			sprintf(E_Val, "%.0f ", standard_value);
			break;
		case 1000:
			sprintf(R_Val, "%.2fk ", resistor_value / 1000);
			sprintf(E_Val, "%.1fk", standard_value / 1000);
			break;
		case 10000:
			sprintf(R_Val, "%.2fk ", resistor_value / 1000);
			sprintf(E_Val, "%.0fk ", standard_value / 1000);
			break;
		case 100000:
			sprintf(R_Val, "%.2fk", resistor_value / 1000);
			sprintf(E_Val, "%.0fk", standard_value / 1000);
			break;
		case 1000000:
			sprintf(R_Val, "%.2fM ", resistor_value / 1000000);
			sprintf(E_Val, "%.1fM", standard_value / 1000000);
			break;
		case 10000000:
			sprintf(R_Val, "%.2fM ", resistor_value / 1000000);
			sprintf(E_Val, "%.0fM ", standard_value / 1000000);
			break;
		default:
			sprintf(R_Val, "%.2f  ", resistor_value);
			sprintf(E_Val, "%.1f ", standard_value);
			break;

	}
}
