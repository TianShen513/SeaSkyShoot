/*
 *  Created on: Jan 27, 2023
 *      Author: Joe Krachey
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "spi.h"

/* IO Pins EEPROM*/
#define PIN_IO_EEPROM_HOLD	P9_3
#define PIN_IO_EEPROM_WP	P9_4
#define PIN_SPI_EEPROM_CS	P7_3


#define EEPROM_CMD_WREN					0x06
#define EEPROM_CMD_WRDI					0x04
#define EEPROM_CMD_RDSR					0x05
#define EEPROM_CMD_WRSR					0x01
#define EEPROM_CMD_READ					0x03
#define EEPROM_CMD_WRITE				0x02
#define EEPROM_CMD_RDID					0x83
#define EEPROM_CMD_WRID					0x82
#define EEPROM_CMD_RDLS					0x83
#define EEPROM_CMD_LID 					0x82

/** Initializes the IO pins used to control the CS of the
 *  EEPROM
 *
 * @param
 *
 */
void eeprom_cs_init(void);

/** Determine if the EEPROM is busy writing the last
 *  transaction to non-volatile storage
 *
 * @param
 *
 */
void eeprom_wait_for_write(void);

/** Enables Writes to the EEPROM
 *
 * @param
 *
 */
void eeprom_write_enable(void);

/** Disable Writes to the EEPROM
 *
 * @param
 *
 */
void eeprom_write_disable(void);


/** Writes a single byte to the specified address
 *
 * @param address -- 16 bit address in the EEPROM
 * @param data    -- value to write into memory
 *
 */
void eeprom_write_byte(uint16_t address, uint8_t data);

/** Reads a single byte to the specified address
 *
 * @param address -- 16 bit address in the EEPROM
 *
 */
uint8_t eeprom_read_byte(uint16_t address);

#endif /* EEPROM_H_ */
