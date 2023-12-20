/*
 *  Created on: Jan 18, 2022
 *      Author: Joe Krachey
 */

#ifndef SPI_H__
#define SPI_H__

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

/* Macros */
#define SPI_FREQ			1000000

/* SPI Pins*/
#define PIN_SPI_MOSI		P7_0
#define PIN_SPI_MISO		P7_1
#define PIN_SPI_CLK		    P7_2

/* Public Global Variables */
extern cyhal_spi_t mSPI;



/* Public API */
cy_rslt_t spi_init(void);

#endif 
