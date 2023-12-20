/*
 * This file should NOT be modified.
 */

#ifndef __TIC_TAC_TOE_IMAGES_H__
#define __TIC_TAC_TOE_IMAGES_H__

#include <stdint.h>

#define X_WIDTH     19
#define X_HEIGHT    25
extern const uint8_t Bitmaps_X[];

#define O_WIDTH     19
#define O_HEIGHT    25
extern const uint8_t Bitmaps_O[];

extern const uint8_t tieBitmaps[];
extern const uint8_t tieWidthPixels;
extern const uint8_t tieHeightPixels;


extern const uint8_t x_WinsBitmaps[];
extern const uint8_t x_WinsWidthPixels;
extern const uint8_t x_WinsHeightPixels;


extern const uint8_t o_WinsBitmaps[]; 
extern const uint8_t o_WinsWidthPixels;
extern const uint8_t o_WinsHeightPixels;

extern const uint8_t SelectPlayer1Bitmaps[] ;
extern const uint8_t SelectPlayer1WidthPixels;
extern const uint8_t SelectPlayer1HeightPixels;

extern const uint8_t WaitForOtherPlayerBitmaps[];
extern const uint8_t WaitForOtherPlayerWidthPixels;
extern const uint8_t WaitForOtherPlayerHeightPixels;

#endif
