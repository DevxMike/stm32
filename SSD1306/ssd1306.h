/*
 * ssd1306.h
 *
 *  Created on: Apr 8, 2022
 *      Author: Michael
 */

#ifndef INC_SSD1306_SSD1306_H_
#define INC_SSD1306_SSD1306_H_

#include "ssd1306_defs.h"

class ssd1306{
private:
	struct{
		bool on_off_state;
		uint8_t x, y;
	}display_current_values;

	I2C_HandleTypeDef& i2c_handle;
	const uint8_t slave_address;
	uint8_t (*buffer)[ssd1306_defs::width];

	void write_command(uint8_t cmd);
	void write_data(uint8_t* data, size_t len);
	void update_display();

public:
	ssd1306(I2C_HandleTypeDef& hi2c, uint8_t slave_addr, uint8_t (*buffer)[ssd1306_defs::width]);
	void draw_pixel(uint8_t x, uint8_t y, uint8_t color);
	void putc(char c, uint8_t color);
	void puts(char* str, uint8_t color);
	void set_cursor(uint8_t x, uint8_t y);
	void clear_display();
	void write_page(uint8_t page_num, uint8_t* data, uint8_t len); //use this function in threads
	uint8_t* get_page(uint8_t);
	void ignore_ram(){ write_command(0xA5); };
	void resume_ram(){ write_command(0xA4); };

	void fill(uint8_t color);
};

#endif /* INC_SSD1306_SSD1306_H_ */
