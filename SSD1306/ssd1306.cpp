/*
 * ssd1306.cpp
 *
 *  Created on: Apr 8, 2022
 *      Author: Michael
 */

#include "SSD1306/ssd1306.h"
#include "cmsis_os.h"

void ssd1306_delay(ms){ //if ure not using freeRTOS change this definition
	osDelay(ms);
}

ssd1306::ssd1306(I2C_HandleTypeDef& hi2c, uint8_t slave_addr): i2c_handle{ hi2c }, slave_address{ slave_addr } {
	display_current_values.on_off_state = false;            //set initial values
	display_current_values.x = display_current_values.y = 0;

	for(uint8_t i = 0; i < ssd1306_defs::width; ++i){     //black out the buffer
		for(uint8_t j = 0; j < ssd1306_defs::height; ++j){
			buffer[i][j] = ssd1306_defs::black;
		}
	}

	//to do: init procedure
}

void ssd1306::write_command(uint8_t cmd){
	HAL_I2C_Mem_Write(
			&i2c_handle,
			slave_address,
			ssd1306_defs::ctrl_byte_cmd,
			1,
			&cmd,
			1,
			HAL_MAX_DELAY
	);
}

void ssd1306::write_data(uint8_t* data, size_t len){
	HAL_I2C_Mem_Write(
			&i2c_handle,
			slave_address,
			ssd1306_defs::ctrl_byte_data,
			1,
			data,
			len,
			HAL_MAX_DELAY
	);
}

