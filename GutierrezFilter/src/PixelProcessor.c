/**
* pixel class to shift a pixel, blur pixels, and create holes in image
*
* Completion time: 5
*
* @author Gutierrez
* @version 2.0
*/

#include <stdio.h>
#include <stdlib.h>
#include "PixelProcessor.h"
#include <math.h>
#include <time.h>

void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (pArr[i][j].blue + bShift > 255) {
                pArr[i][j].blue = 255;
            }
            else if (pArr[i][j].blue + bShift < 0) {
                pArr[i][j].blue = 0;
            } else {
                pArr[i][j].blue += bShift;
            }

            if (pArr[i][j].green + gShift > 255) {
                pArr[i][j].green = 255;
            } else if (pArr[i][j].green + gShift < 0) {
                pArr[i][j].green = 0;
            } else {
                pArr[i][j].green += + gShift;
            }

            if (pArr[i][j].red + rShift > 255) {
                pArr[i][j].red = 255;
            } else if (pArr[i][j].red + rShift < 0) {
                pArr[i][j].red = 0;
            } else {
                pArr[i][j].red += + rShift;
            }
        }
    }
}

void blurPixels(Pixel** pArr, Pixel** pArrCopy, int width, int height, int start, int end) {
    int count = 9;
    for (int i = 0; i < height; ++i) {
        for (int j = start; j < end +2; ++j) {
            int sumBlue = 0;
            int sumRed = 0;
            int sumGreen = 0;
            for (int k = -1; k < 2; ++k) {
                for (int l = -1; l < 2; ++l) {
                    if(i+k >= 0 && i+k < height &&
                            j+l >= 0 && j+k < end) {
                        sumBlue += pArr[i + k][j + l].blue;
                        sumRed += pArr[i + k][j + l].red;
                        sumGreen += pArr[i + k][j + l].green;
                    }
                }
            }

            pArrCopy[i][j].blue = sumBlue / count;
            pArrCopy[i][j].red = sumRed / count;
            pArrCopy[i][j].green = sumGreen / count;
        }
    }
}

void drawCircle(Pixel** pArr, int width, int height, int center_x, int center_y, int radi) {
    int counter = radi;
    for (int i = (center_y-radi); i < (center_y+radi); ++i) { // y
        if (i >= 0 && i < height){
            int x_offset = (int) sqrt((radi * radi - counter * counter));

            if(center_x - x_offset < 0){
                for (int k = 0; k < center_x + x_offset + 1; ++k) {
                    pArr[i][k].blue = 0;
                    pArr[i][k].green = 0;
                    pArr[i][k].red = 0;
                }
            } else if(center_x + x_offset >= width){
                for (int k = center_x - x_offset; k < width; k++) {
                    pArr[i][k].blue = 0;
                    pArr[i][k].green = 0;
                    pArr[i][k].red = 0;
                }
            }else {
                for (int k = center_x - x_offset; k < center_x + x_offset + 1; ++k) {
                    pArr[i][k].blue = 0;
                    pArr[i][k].green = 0;
                    pArr[i][k].red = 0;
                }
            }
        }
        counter--;
    }
}

void swissCheese(Pixel** pArr, int width, int height) {
    int shorterSize = height < width ? height : width;
    int avg = shorterSize/10;
    printf("avg: %d\n", avg);
    srand(time(0));
    for (int i = 0; i < avg; ++i) {
        if (i < avg / 2)
            drawCircle(pArr, width, height, rand() % width, rand() % height, shorterSize/10);
        else if (i < avg * 2/3)
            drawCircle(pArr, width, height, rand() % width, rand() % height, shorterSize/8);
        else
            drawCircle(pArr, width, height, rand() % width, rand() % height, shorterSize/15);

    }
}