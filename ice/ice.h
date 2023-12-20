/**
 * @file ice.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __ICE_H__
#define __ICE_H__

extern char ICE_DESCRIPTION[];

/**
 * @brief 
 * This function will initialize all of the hardware resources for
 * the ICE.  
 * 
 * This function is implemented in the iceXX.c file for the ICE you are
 * working on.
 */
void peripheral_init(void);

/*****************************************************************************/
/* Application Code                                                          */
/*****************************************************************************/
/**
 * @brief 
 * This function implements the behavioral requirements for the ICE
 * 
 * This function is implemented in the iceXX.c file for the ICE you are 
 * working on.
 */
void main_app(void);

#endif