#include "stdio.h"
#include "resistor.h"
#include "i2c-lcd.h"

extern R_paramTypeDef R_config;

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

void screenMeasurements(char writeString[]){
	lcd_put_cur(0, 0);
	sprintf(writeString, "Value:%.3f          ", R_config.r_measured);
	lcd_send_string(writeString);
	lcd_put_cur(1, 0);
	sprintf(writeString, "E-Val:%.0f  Er:%.1f%%   ", R_config.r_standard, R_config.r_percentage);
	lcd_send_string(writeString);
	HAL_Delay(500);
}
