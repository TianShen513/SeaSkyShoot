/**
 * @file spi.c
 * @author Bowen Quan (bquan2@wisc.edu)
 * @brief 
 * @version 0.1
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "spi.h"


cyhal_spi_t mSPI;

cy_rslt_t spi_init(void)
{
	cy_rslt_t   rslt;

	memset(&mSPI, 0, sizeof(mSPI));

    // Configuring the  SPI master:  Specify the SPI interface pins, frame size, SPI Motorola mode
    // and master mode
    rslt = cyhal_spi_init(
    						&mSPI,
							PIN_SPI_MOSI,			// MOSI Pin
							PIN_SPI_MISO,			// MISO Pin
							PIN_SPI_CLK,			// Clock Pin
							NC,						 	// CS -- Will control using an IO pin
							NULL,						// Clock Source -- if not provided a new clock will be allocated
							8,							// Bits per frame
							CYHAL_SPI_MODE_00_MSB,		// SPI Mode
							false						// Is Subordinate??
						);

    if (rslt == CY_RSLT_SUCCESS)
    {
        // Set the data rate to 1 Mbps
    	rslt = cyhal_spi_set_frequency(&mSPI, SPI_FREQ);
    }

	return rslt;

}
