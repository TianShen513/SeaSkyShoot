/*
 ******************************************************************************
 * @file    orientation_6d.c
 * @author  Sensors Software Solution Team
 * @brief   This file show how to detect 6D orientation from sensor.
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/*
 * This example was developed using the following STMicroelectronics
 * evaluation boards:
 *
 * - STEVAL_MKI109V3 + STEVAL-MKI189V1
 * - NUCLEO_F411RE + STEVAL-MKI189V1
 * - DISCOVERY_SPC584B + STEVAL-MKI189V1
 *
 * Used interfaces:
 *
 * STEVAL_MKI109V3    - Host side:   USB (Virtual COM)
 *                    - Sensor side: SPI(Default) / I2C(supported)
 *
 * NUCLEO_STM32F411RE - Host side: UART(COM) to USB bridge
 *                    - I2C(Default) / SPI(supported)
 *
 * DISCOVERY_SPC584B  - Host side: UART(COM) to USB bridge
 *                    - Sensor side: I2C(Default) / SPI(supported)
 *
 * If you need to run this example on a different hardware platform a
 * modification of the functions: `platform_write`, `platform_read`,
 * `tx_com` and 'platform_init' is required.
 *
 */

/* STMicroelectronics evaluation boards definition
 *
 * Please uncomment ONLY the evaluation boards in use.
 * If a different hardware is used please comment all
 * following target board and redefine yours.
 */

// #define STEVAL_MKI109V3  /* little endian */
// #define NUCLEO_F411RE    /* little endian */
// #define SPC584B_DIS      /* big endian */

/* ATTENTION: By default the driver is little endian. If you need switch
 *            to big endian please see "Endianness definitions" in the
 *            header file of the driver (_reg.h).
 */

/* Includes ------------------------------------------------------------------*/
#include "imu.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint8_t whoamI, rst;
static uint8_t tx_buffer[1000];

/* Extern variables ----------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*
 *   WARNING:
 *   Functions declare in this section are defined at the end of this file
 *   and are strictly related to the hardware platform used.
 *
 */
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);
static void tx_com(uint8_t *tx_buffer, uint16_t len);
static void platform_delay(uint32_t ms);
static void platform_init(void);

stmdev_ctx_t dev_ctx;

/* Main Example --------------------------------------------------------------*/
void lsm6dsm_orientation(void)
{
  /* Initialize mems driver interface */ 
  lsm6dsm_int1_route_t int_1_reg;
  /* Uncomment if interrupt generation on 6D INT2 pin */
  // lsm6dsm_int2_route_t int_2_reg;
  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;

  /* Init test platform */
  platform_init();
  /* Wait sensor boot time */
  platform_delay(15);
  /* Check device ID */
  lsm6dsm_device_id_get(&dev_ctx, &whoamI);

  if (whoamI != LSM6DSM_ID)
  {
    printf("Device not found\n\r");
    while (1)
    {
      /* manage here device not found */
    }
  }

  /* Restore default configuration */
  lsm6dsm_reset_set(&dev_ctx, PROPERTY_ENABLE);

  do
  {
    lsm6dsm_reset_get(&dev_ctx, &rst);
  } while (rst);

  /* Set XL Output Data Rate */
  lsm6dsm_xl_data_rate_set(&dev_ctx, LSM6DSM_XL_ODR_416Hz);
  /* Set 2g full XL scale */
  lsm6dsm_xl_full_scale_set(&dev_ctx, LSM6DSM_2g);
  /* Set threshold to 60 degrees */
  lsm6dsm_6d_threshold_set(&dev_ctx, LSM6DSM_DEG_60);
  /* Use HP filter */
  lsm6dsm_xl_hp_path_internal_set(&dev_ctx, LSM6DSM_USE_HPF);
  /* LPF2 on 6D function selection */
  lsm6dsm_6d_feed_data_set(&dev_ctx, LSM6DSM_LPF2_FEED);
  /* Enable interrupt generation on 6D INT1 pin */
  lsm6dsm_pin_int1_route_get(&dev_ctx, &int_1_reg);
  int_1_reg.int1_6d = PROPERTY_ENABLE;
  lsm6dsm_pin_int1_route_set(&dev_ctx, int_1_reg);

  // configure accelerometer
  lsm6dsm_odr_xl_t acc_data_rate = LSM6DSM_XL_ODR_1k66Hz;
  lsm6dsm_xl_data_rate_set(&dev_ctx, acc_data_rate);


  // /* Wait Events */
  // while (1)
  // {
  //   lsm6dsm_all_sources_t all_source;

  //   /* Check if 6D Orientation events */
  //   lsm6dsm_all_sources_get(&dev_ctx, &all_source);

  //   if (all_source.d6d_src.d6d_ia)
  //   {
  //     sprintf((char *)tx_buffer, "Orientation:  ");

  //     if (all_source.d6d_src.xh)
  //     {
  //       strcat((char *)tx_buffer, "XH");
  //     }

  //     if (all_source.d6d_src.xl)
  //     {
  //       strcat((char *)tx_buffer, "XL");
  //     }

  //     if (all_source.d6d_src.yh)
  //     {
  //       strcat((char *)tx_buffer, "YH");
  //     }

  //     if (all_source.d6d_src.yl)
  //     {
  //       strcat((char *)tx_buffer, "YL");
  //     }

  //     if (all_source.d6d_src.zh)
  //     {
  //       strcat((char *)tx_buffer, "ZH");
  //     }

  //     if (all_source.d6d_src.zl)
  //     {
  //       strcat((char *)tx_buffer, "ZL");
  //     }

  //     strcat((char *)tx_buffer, "\r\n");
  //     tx_com(tx_buffer, strlen((char const *)tx_buffer));
  //     cyhal_system_delay_ms(50);
  //   }
  // }
}

void get_acceleration(imu_acceleration_t *accel) {
  int16_t buffer[3];
  lsm6dsm_acceleration_raw_get(&dev_ctx, buffer);
  accel->x = buffer[0];
  accel->y = buffer[1];
  accel->z = buffer[2];
}

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len)
{
  cy_rslt_t rslt;
  uint8_t *tx;
  uint8_t *rx;

  /* Allocate memory for the spi Tx message */
  tx = malloc(sizeof(uint8_t) * (len + 1));

  /* Allocate memory for the spi Rx message */
  rx = malloc(sizeof(uint8_t) * (len + 1));

  // Write the register address to the first memory location in the Tx buffer
  tx[0] = reg & 0x7F; // make MSB 0 for write

  // Copy the remaining bytes to the Tx message
  memcpy(&tx[1], bufp, len);

  // Set the CS Low
  cyhal_gpio_write(PIN_SPI_IMU_CS, 0);

  // Starts a data transfer
  rslt = cyhal_spi_transfer(
    &mSPI,
    tx,
    len + 1,
    rx,
    len + 1,
    0x00
  );

  // Set the CS High
    cyhal_gpio_write(PIN_SPI_IMU_CS, 1);

  free(tx);
  free(rx);

  return (uint32_t)rslt;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len)
{
  cy_rslt_t rslt;
  uint8_t *tx;
  uint8_t *rx;

  /* Allocate memory for the spi Tx message */
  tx = malloc(sizeof(uint8_t) * len + 1);

  /* Allocate memory for the spi Rx message */
  rx = malloc(sizeof(uint8_t) * len + 1);

  // Write the register address to the first memory location in the Tx Message
  tx[0] = reg | 0x80; // set MSB to 1 for read 

  // set the remaining bytes in tx to 0
  memset(&tx[1], 0, len);

  // Set the CS Low
  cyhal_gpio_write(PIN_SPI_IMU_CS, 0);

  // Starts a data transfer
  rslt = cyhal_spi_transfer(
    &mSPI,
    tx,
    len + 1,
    rx,
    len + 1,
    0x00
  );

  // Set the CS High
  cyhal_gpio_write(PIN_SPI_IMU_CS, 1);

  // Copy the data returned from IMU to the destination address
  memcpy(bufp, (rx + 1), len);

  free(tx);
  free(rx);

  return (uint32_t)rslt;
}

/*
 * @brief  Send buffer to console (platform dependent)
 *
 * @param  tx_buffer     buffer to transmit
 * @param  len           number of byte to send
 *
 */
static void tx_com(uint8_t *tx_buffer, uint16_t len)
{
  printf("%s", tx_buffer);
}

/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */
static void platform_delay(uint32_t ms)
{
  Cy_SysLib_Delay(ms);
}

/*
 * @brief  platform specific initialization (platform dependent)
 */
static void platform_init(void)
{
  cyhal_gpio_init(
      PIN_SPI_IMU_CS,          // Pin
      CYHAL_GPIO_DIR_OUTPUT,   // Direction
      CYHAL_GPIO_DRIVE_STRONG, // Drive Mode
      true);                   // InitialValue
}