/*
 * ssd1306.cpp
 *
 *  Created on: Apr 8, 2022
 *      Author: Michael
 */

#include "SSD1306/ssd1306.h"
#include "cmsis_os.h"
#include "SSD1306/font.h"

ssd1306::ssd1306(I2C_HandleTypeDef& hi2c, uint8_t slave_addr, uint8_t (*buff)[ssd1306_defs::width]): i2c_handle{ hi2c }, slave_address{ slave_addr }, buffer{ buff } {
	display_current_values.on_off_state = false;            //set initial values
	display_current_values.x = display_current_values.y = 0;

	for(uint8_t i = 0; i <  ssd1306_defs::height /ssd1306_defs::page_count; ++i){     //black out the buffer
		for(uint8_t j = 0; j < ssd1306_defs::width; ++j){
			buffer[i][j] = 0x00; //( i & 1 ? 0x00 : 0xFF);
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

uint8_t* ssd1306::get_page(uint8_t page){
	if(page < ssd1306_defs::page_count){
		return buffer[page];
	}

	return NULL;
}

void ssd1306::write_page(uint8_t page_num, uint8_t* data, uint8_t len){
	uint8_t ram_page = page_num + ssd1306_defs::page_start;

	write_command(ram_page); //set page
	write_command(ssd1306_defs::lower_column);
	write_command(ssd1306_defs::higher_column);

	write_data(data, len);
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

void ssd1306::clear_display(){
	for(uint8_t i = 0; i < ssd1306_defs::height /ssd1306_defs::page_count; ++i){     //black out the buffer
		for(uint8_t j = 0; j < ssd1306_defs::width; ++j){
			buffer[i][j] = 0x00;
		}
	}

	display_current_values.x = display_current_values.y = 0;

	update_display();
}

void ssd1306::putc(char c, uint8_t color){
	static uint8_t* letter;

	if(c >= 32 && c <= 126){
		uint8_t index = c - 32;

		uint8_t tmp = (color == ssd1306_defs::white? 0xFF : 0x00);

		letter = (uint8_t*)Font6x8[index];

		if(display_current_values.x + ssd1306_defs::font_width > ssd1306_defs::width){
			display_current_values.x = 0; display_current_values.y += ssd1306_defs::font_height + 5;
		}
		if(display_current_values.y + ssd1306_defs::font_height > ssd1306_defs::height){
			clear_display();
		}

		//buffer[display_current_values.y / ssd1306_defs::font_width][display_current_values.x++] = letter[i] & tmp;

		for(uint8_t i = 0; i < ssd1306_defs::font_width; ++i){
			buffer[display_current_values.y / ssd1306_defs::font_width][display_current_values.x++] = letter[i] & tmp;
		}

		update_display();
	}
}

void ssd1306::puts(char* str, uint8_t color){
	while(*str){
		putc(*str, color); ++str;
	}
}

void ssd1306::fill(uint8_t color){
	uint8_t tmp;

	if(color == ssd1306_defs::white){
		tmp = 0xFF;
	}
	else{
		tmp = 0x00;
	}

	for(uint8_t i = 0; i < ssd1306_defs::height /ssd1306_defs::page_count; ++i){     //black out the buffer
		for(uint8_t j = 0; j < ssd1306_defs::width; ++j){
			buffer[i][j] = tmp;
		}
	}

	update_display();
}
