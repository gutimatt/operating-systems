/**
* pixel class to shift a pixel
*
* Completion time: 1
*
* @author Gutierrez
* @version 1.0
*/

#include "../headers/PixelProcessor.h"
#include <stdio.h>


void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (pArr[i][j].b + bShift > 255) {
                pArr[i][j].b = 255;
            }
            else if (pArr[i][j].b + bShift < 0) {
                pArr[i][j].b = 0;
            } else {
                pArr[i][j].b += bShift;
            }

            if (pArr[i][j].g + gShift > 255) {
                pArr[i][j].g = 255;
            } else if (pArr[i][j].g + gShift < 0) {
                pArr[i][j].g = 0;
            } else {
                pArr[i][j].g += + gShift;
            }

            if (pArr[i][j].r + rShift > 255) {
                pArr[i][j].r = 255;
            } else if (pArr[i][j].r + rShift < 0) {
                pArr[i][j].r = 0;
            } else {
                pArr[i][j].r += + rShift;

            }
        }
    }
}
