/*
 * ssd1306.cpp
 *
 *  Created on: Apr 8, 2022
 *      Author: Michael
 */

#include "SSD1306/ssd1306.h"
#include "cmsis_os.h"

ssd1306::ssd1306(I2C_HandleTypeDef& hi2c, uint8_t slave_addr): i2c_handle{ hi2c }, slave_address{ slave_addr } {
	display_current_values.on_off_state = false;            //set initial values
	display_current_values.x = display_current_values.y = 0;

	for(uint8_t i = 0; i < ssd1306_defs::height; ++i){     //black out the buffer
		for(uint8_t j = 0; j < ssd1306_defs::width; ++j){
			buffer[i][j] = ( i & 1 ? 0x00 : 0xFF);
		}
	}

	//to do: init procedure

	write_command(ssd1306_defs::display_off);

	write_command(ssd1306_defs::addr_mode);
	write_command(0x00);

	write_command(ssd1306_defs::page_start);

	write_command(ssd1306_defs::scan_direction);

	write_command(ssd1306_defs::lower_column);
	write_command(ssd1306_defs::higher_column);

	write_command(ssd1306_defs::start_line);

	write_command(ssd1306_defs::segment_remap);

	write_command(ssd1306_defs::set_contrast);
	write_command(0xFF); //max contrast

	write_command(ssd1306_defs::normal_color);

	write_command(ssd1306_defs::mux_ratio);
	write_command(0x3F);

	write_command(ssd1306_defs::out_follows_ram);

	write_command(ssd1306_defs::display_offset);
	write_command(0x00);

	write_command(ssd1306_defs::display_clk);
	write_command(0x80);

	write_command(ssd1306_defs::pre_charge);
	write_command(0x22);

	write_command(ssd1306_defs::com_config);
	write_command(0x12);

	write_command(ssd1306_defs::vcomh);
	write_command(0x20);

	write_command(ssd1306_defs::dc_enable);
	write_command(0x14);

	write_command(ssd1306_defs::display_on);

	update_display();
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

void ssd1306::set_cursor(uint8_t x, uint8_t y){
	display_current_values.x = x;
	display_current_values.y = y;
}

void ssd1306::update_display(){
	for(uint8_t page = 0; page < ssd1306_defs::height / ssd1306_defs::page_count; ++page){
		uint8_t ram_page = page + ssd1306_defs::page_start;

		write_command(ram_page); //set page
		write_command(ssd1306_defs::lower_column);
		write_command(ssd1306_defs::higher_column);

		write_data(buffer[page], ssd1306_defs::width);
	}
}
