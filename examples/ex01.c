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

#if defined(EXAMPLE) && (FILE_ID == 1)

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
    uint8_t count = 0;
    uint8_t size = 20;
    printf("Hello Class\n\r");

    /* Initialize ArrayA to Random Values*/
    for (uint8_t i = 0; i < size; i++)
    {
        ArrayA[i] = rand();
    }
    
    
    /* Initialize ArrayB to Random Values*/

    for (uint8_t i = 0; i < size; i++)
    {
        ArrayB[i] = rand();
    }

    /* Count the number of times ArrayA[i] > ArrayB[i]*/
    for (uint8_t i = 0; i < size; i++)
    {
        if(ArrayA[i] > ArrayB[i])
        {
            count++;
        }
    }

    /* Print out the count to the console */
    printf("Count is equal %d\n\r", count);

    /* Enter Infinite Loop*/
    while (1)
    {
    }
}
#endif