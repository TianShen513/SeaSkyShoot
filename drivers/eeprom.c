/**
 * @file eeprom.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-10-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "eeprom.h"
#include <stdio.h>

/** Initializes the IO pins used to control the CS of the
 *  EEPROM
 *
 * @param
 *
 */
void eeprom_cs_init(void)
{
	cy_rslt_t rslt;

	/* ADD CODE */
	/* Initialize EEPROM Chip Select IO pins as an output*/
	rslt =  cyhal_gpio_init(
      PIN_SPI_EEPROM_CS,          // Pin
      CYHAL_GPIO_DIR_OUTPUT,   // Direction
      CYHAL_GPIO_DRIVE_STRONG, // Drive Mode
      0);                   // InitialValue

	CY_ASSERT(rslt == CY_RSLT_SUCCESS); /* Halt MCU if init fails*/
}

/** Determine if the EEPROM is busy writing the last
 *  transaction to non-volatile storage
 *
 * @param
 *
 */
void eeprom_wait_for_write(void)
{
	/* ADD CODE */
	uint8_t transmit_data[2] = {EEPROM_CMD_RDSR, 0};
	uint8_t receive_data[2] = {0, 0};
	cy_rslt_t rslt;

	// Check to see if the eeprom is still updating
	// the data from the last write
	do
	{
		/* ADD CODE */
		/* Set the CS Low */
		//printf("line 53 \n");
		cyhal_gpio_write(PIN_SPI_EEPROM_CS, 0);
		// Starts a data transfer
		rslt = cyhal_spi_transfer(&mSPI, transmit_data, sizeof(transmit_data), receive_data, sizeof(receive_data), 0x00);
		CY_ASSERT(rslt == CY_RSLT_SUCCESS); /* Halt MCU if SPI transaction fails*/

		// Set the CS High
		cyhal_gpio_write(PIN_SPI_EEPROM_CS, 1);
		// If the address was not ACKed, try again.
	} while (receive_data[1] & 0x01 );
	//printf("line 63 \n");
}

/** Enables Writes to the EEPROM
 *
 * @param
 *
 */
void eeprom_write_enable(void)
{
	/* ADD CODE */
	uint8_t transmit_data[1] 	= {EEPROM_CMD_WREN};
	uint8_t receive_data[1] 	= {0};
	cy_rslt_t rslt;

	// Set the CS Low
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 0);
	// Starts a data transfer
	rslt = cyhal_spi_transfer(&mSPI, transmit_data, sizeof(transmit_data), receive_data, sizeof(receive_data), 0x00);
	CY_ASSERT(rslt == CY_RSLT_SUCCESS); /* Halt MCU if SPI transaction fails*/

	// Set the CS High
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 1);
}

/** Disable Writes to the EEPROM
 *
 * @param
 *
 */
void eeprom_write_disable(void)
{
	/* ADD CODE */
	uint8_t transmit_data[1] 	= {EEPROM_CMD_WRDI};
	uint8_t receive_data[1] 	= {0};
	cy_rslt_t rslt;

	// Set the CS Low
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 0);
	// Starts a data transfer
	rslt = cyhal_spi_transfer(&mSPI, transmit_data, sizeof(transmit_data), receive_data, sizeof(receive_data), 0x00);
	CY_ASSERT(rslt == CY_RSLT_SUCCESS); /* Halt MCU if SPI transaction fails*/

	// Set the CS High
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 1);
}

/** Writes a single byte to the specified address
 *
 * @param address -- 16 bit address in the EEPROM
 * @param data    -- value to write into memory
 *
 */
void eeprom_write_byte(uint16_t address, uint8_t data)
{
	/* ADD CODE */
	uint8_t transmit_data[4] 	= {EEPROM_CMD_WRITE, (address >> 8) & 0xFF, address & 0xFF,data };
	uint8_t receive_data[4] 	= {0, 0, 0, 0};
	cy_rslt_t rslt;

	// Wait for any outstanding writes to complete
	eeprom_wait_for_write();

	// Enable writes to the eeprom
	eeprom_write_enable();

// Set the CS Low
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 0);
	// Starts a data transfer
	rslt = cyhal_spi_transfer(&mSPI, transmit_data, sizeof(transmit_data), receive_data, sizeof(receive_data), 0x00);
	CY_ASSERT(rslt == CY_RSLT_SUCCESS); /* Halt MCU if SPI transaction fails*/

	// Set the CS High
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 1);

	// Disable writes to the eeprom
	eeprom_write_disable();
}

/** Reads a single byte to the specified address
 *
 * @param address -- 16 bit address in the EEPROM
 *
 */
uint8_t eeprom_read_byte(uint16_t address)
{
	/* ADD CODE */
	uint8_t transmit_data[4] 	= {EEPROM_CMD_READ, (address >> 8) & 0xFF, address & 0xFF, 0};
	uint8_t receive_data[4] 	= {0, 0, 0, 0};
	cy_rslt_t rslt;

	// Wait for any outstanding writes to complete
	eeprom_wait_for_write();

	// Enable writes to the eeprom
	eeprom_write_enable();

	// Set the CS Low
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 0);

	// Starts a data transfer
	rslt = cyhal_spi_transfer(&mSPI, transmit_data, sizeof(transmit_data), receive_data, sizeof(receive_data), 0x00);
	CY_ASSERT(rslt == CY_RSLT_SUCCESS); /* Halt MCU if SPI transaction fails*/

	// Set the CS High
	cyhal_gpio_write(PIN_SPI_EEPROM_CS, 1);

	// Disable writes to the eeprom
	eeprom_write_disable();

	// Return the value from the EEPROM to the user
	return receive_data[3];
}