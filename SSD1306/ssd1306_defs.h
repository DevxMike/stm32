/*
 * ssd1306_defs.h
 *
 *  Created on: Apr 8, 2022
 *      Author: Michael
 */

#ifndef INC_SSD1306_SSD1306_DEFS_H_
#define INC_SSD1306_SSD1306_DEFS_H_

//HAL
#include "main.h"

namespace ssd1306_defs{
	//last two bits stand for: SA0 (Data = 1 or CMD = 0), RW (Read = 1, Write = 0)
	constexpr uint8_t slave_address = 0b01111000;
	constexpr uint8_t height = 64;
	constexpr uint8_t width = 128;
	constexpr uint8_t page_count = 8;

	enum{
		black = 0x00, white = 0x01
	}; //colors available

	enum{
		ctrl_byte_cmd = 0x00, ctrl_byte_data = 0x40
	};

	enum{
		lower_column = 0x00, higher_column = 0x10,
		addr_mode = 0x20,
		column_address = 0x21,
		start_line = 0x40,
		set_contrast = 0x81,
		segment_remap = 0xA0,
		out_follows_ram = 0xA4,
		normal_color = 0xA6,
		mux_ratio = 0xA8,
		display_off = 0xAE, display_on = 0xAF,
		page_start = 0xB0, scan_direction = 0xC0,
		display_offset = 0xD3, display_clk = 0xD5,
		pre_charge = 0xD9,
		com_config = 0xDA,
		vcomh = 0xDB,
		dc_enable = 0x8D
	};
}


#endif /* INC_SSD1306_SSD1306_DEFS_H_ */
