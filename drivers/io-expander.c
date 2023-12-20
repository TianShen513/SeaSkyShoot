/**
 * @file io-expander.c
 * @author Joe Krachey (jkrachey@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-10-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "io-expander.h"

/** Write a register on the TCA9534
 *
 * @param reg The reg address to read
 * @param value The value to be written
 *
 */
static void io_expander_write_reg(uint8_t reg, uint8_t value)
{
	cy_rslt_t rslt;

	/* ADD CODE */
	/* Allocate the write data packet that will be sent to the IO Expander */
	uint8_t write_data[2] = {reg, value};

	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	rslt = cyhal_i2c_master_write(&i2c_monarch_obj, 
								LTR_329ALS_01_ADDR,
								write_data, // address of array
								2, // control byte is implied, so we send 2 bytes for the register address and write value
								0, // wait until i2c write succeeds
								true); // send STOP

	CY_ASSERT(rslt == CY_RSLT_SUCCESS); /* If the write failed, halt the CPU */
}

/** Read a register on the TCA9534
 *
 * @param reg The reg address to read
 *
 */
static uint8_t io_expander_read_reg(uint8_t reg)
{
	cy_rslt_t rslt;

	/* ADD CODE */
	/* Allocate the write data packet that will be sent to the IO Expander */
	uint8_t write_data[1] = {reg};
	
	/* ADD CODE */
	/* Allocate the read data packet that will be sent from the IO Expander */
	uint8_t read_data[1] = {0};

	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	rslt = cyhal_i2c_master_write(&i2c_monarch_obj, 
								LTR_329ALS_01_ADDR,
								write_data, // address of array
								1, // control byte is implied, so we send 2 bytes for the register address and write value
								0, // wait until i2c write succeeds
								true); // send STOP
	
	CY_ASSERT(rslt == CY_RSLT_SUCCESS);

	/* ADD CODE */
	/* Use cyhal_i2c_master_read to read the required data from the device. */
	rslt = cyhal_i2c_master_read(&i2c_monarch_obj, 
								LTR_329ALS_01_ADDR, 
								read_data, 
								1, // read 1 byte of data
								0,
								true);
	
	CY_ASSERT(rslt == CY_RSLT_SUCCESS);

	return read_data[0];
}

/** Read a register on the TCA9534
 *
 * @param reg The reg address to read
 *
 */
uint8_t io_read_reg(uint8_t reg)
{
	cy_rslt_t rslt;

	/* ADD CODE */
	/* Allocate the write data packet that will be sent to the IO Expander */
	uint8_t write_data[1] = {reg};
	
	/* ADD CODE */
	/* Allocate the read data packet that will be sent from the IO Expander */
	uint8_t read_data[1] = {0};

	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	rslt = cyhal_i2c_master_write(&i2c_monarch_obj, 
								LTR_329ALS_01_ADDR,
								write_data, // address of array
								1, // control byte is implied, so we send 2 bytes for the register address and write value
								0, // wait until i2c write succeeds
								true); // send STOP
	
	CY_ASSERT(rslt == CY_RSLT_SUCCESS);

	/* ADD CODE */
	/* Use cyhal_i2c_master_read to read the required data from the device. */
	rslt = cyhal_i2c_master_read(&i2c_monarch_obj, 
								LTR_329ALS_01_ADDR, 
								read_data, 
								1, // read 1 byte of data
								0,
								true);
	
	CY_ASSERT(rslt == CY_RSLT_SUCCESS);

	return read_data[0];
}


/**
 * Read the value from config port
*/
uint8_t io_expander_get_contr_port(void){
	return io_expander_read_reg(LTR_ALS_CONTR_ADDR);
}


/**
 * Write the value of the config port
*/
void io_expander_set_contr_port(uint8_t value){
	io_expander_write_reg(LTR_ALS_CONTR_ADDR, value);
}

/**
 * read the measurement rate port
*/
uint8_t io_expander_get_meas_port(void){
	return io_expander_read_reg(LTR_ALS_MEAS_RATE);
}

/**
 * set the measurement rate port
*/
void io_expander_set_meas_port(uint8_t value){
	io_expander_write_reg(LTR_ALS_MEAS_RATE, value);
}


static void io_expander_writeled_reg(uint8_t reg, uint8_t value)
{
	cy_rslt_t rslt;
	uint8_t write_data[2] = {reg, value};
	/* ADD CODE */
	/* Allocate the write data packet that will be sent to the IO Expander */
	rslt = cyhal_i2c_master_write(&i2c_monarch_obj, TCA9534_SUBORDINATE_ADDR, write_data, 2, 0, true);
	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	CY_ASSERT(rslt == CY_RSLT_SUCCESS);
}

/** Read a register on the TCA9534
 *
 * @param reg The reg address to read
 *
 */
static uint8_t io_expander_readled_reg(uint8_t reg)
{
	cy_rslt_t rslt;

	/* ADD CODE */
	/* Allocate the write data packet that will be sent to the IO Expander */
	uint8_t write_data[1] = {reg};
	/* ADD CODE */
	/* Allocate the read data packet that will be sent from the IO Expander */
	uint8_t read_data[2];
	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	rslt = cyhal_i2c_master_write(&i2c_monarch_obj, TCA9534_SUBORDINATE_ADDR, write_data, 1, 0, true);
	CY_ASSERT(rslt == CY_RSLT_SUCCESS);

	/* ADD CODE */
	/* Use cyhal_i2c_master_read to read the required data from the device. */
	rslt = cyhal_i2c_master_read(&i2c_monarch_obj, TCA9534_SUBORDINATE_ADDR, read_data, 2, 0, true);
	CY_ASSERT(rslt == CY_RSLT_SUCCESS);

	

	
	return read_data[0];
}

/** Read the value of the input port
 *
 * @param reg The reg address to read
 *
 */
uint8_t io_expander_get_input_port(void)
{
	return io_expander_readled_reg(TCA9534_INPUT_PORT_ADDR);

}

/** Write the value of the output port
 *
 * @param value The value written
 *
 */
void io_expander_set_output_port(uint8_t value)
{
	io_expander_writeled_reg(TCA9534_OUTPUT_PORT_ADDR, value);
}

/** Write the polarity inversion register
 *
 * @param value The value written
 *
 */
void io_expander_set_polarity_inversion(uint8_t value)
{
	io_expander_writeled_reg(TCA9534_POLARITY_ADDR, value);
}

/** Write the value of the configuration Register
 *
 * @param value The value written
 *
 */
void io_expander_set_configuration(uint8_t value)
{
	io_expander_writeled_reg(TCA9534_CONFIG_ADDR, value);
}


void io_expander_led_red(void)
{

	uint8_t io_sw = 0xFF;
	/* Need to read the IO Expander due to the way Interrupts from the IO Expander work */
	io_sw = io_expander_get_input_port();

	/* Set the IO Expander Outputs to the active LED */
	// LED index 0x01 0x02 0x04 0x08 0x10 0x20 0x40
	
	io_expander_set_output_port(0x02);


}

void io_expander_led_grn(void)
{

	uint8_t io_sw = 0xFF;
	/* Need to read the IO Expander due to the way Interrupts from the IO Expander work */
	io_sw = io_expander_get_input_port();

	/* Set the IO Expander Outputs to the active LED */
	// LED index 0x01 0x02 0x04 0x08 0x10 0x20 0x40
	io_expander_set_output_port(0x08);
	


}
