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

	enum{
		black = 0x00, white = 0x01
	}; //colors available

	enum{
		ctrl_byte_cmd = 0x00, ctrl_byte_data = 0x40
	};

	enum{
		horizontal_addr_mode = 0x20,
		page_start = 0xB0,
		display_off = 0xAE, display_on = 0xAF
	};
}


#endif /* INC_SSD1306_SSD1306_DEFS_H_ */
