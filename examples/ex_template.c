/**
 * @file ex01.c
 * @author Bowen Quan bquan2@wisc.edu
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../main.h"

#if defined(EXAMPLE) && (FILE_ID == 0)

/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: Ex01 - Intro to C";
uint8_t ArrayA[20];
uint8_t ArrayB[20];

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 * This function will initialize hardware resources 
 */
void peripheral_init(void)
{
    /* There are no hardware resources to initialize*/
}

/**
 * @brief
 * This function implements the behavioral requirements for the application
 */
void main_app(void)
{
    /* Enter Infinite Loop*/
    while (1)
    {
    }
}
#endif