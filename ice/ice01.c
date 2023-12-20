/**
 * @file ice07.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <string.h>
#include "../main.h"

#if defined(ICE) && (FILE_ID == 1)
/*****************************************************************************/
/* Macros                                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/
char ICE_DESCRIPTION[] = "ECE353: ICE 01 - Intro to C";

/*****************************************************************************/
/* Function Declarations                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function Definitions                                                      */
/*****************************************************************************/

/**
 * @brief
 * This function will initialize all of the hardware resources for
 * the ICE
 */
void peripheral_init(void)
{
    /* There are no hardware resources to initialize in ICE01 */
}

/**
 * @brief
 * This function implements the behavioral requirements for the ICE
 */
void main_app(void)
{
    printf("!! Student Name (CAPS):");
    for (int i = 0; i < strlen(NAME); i++) {
        printf("%c", toupper(NAME[i]));
    }
    printf(" !!\n");
    /* Enter Infinite Loop*/
    while (1)
    {
    }
}
#endif