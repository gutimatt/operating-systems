/**
* A utility functions for doing pixel transformations.
*
* Completion time: -1 minutes
*
* @author Vatrcia Edgar
* @version 1.0
*/

#ifndef PixelProcessor_H
#define PixelProcessor_H 1
typedef struct Pixel{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}Pixel;

//NOT NEEDED FOR THREADING HW.
void colorShiftPixels(Pixel** pArr, int width, int height, int rShift, int gShift, int bShift);

void blurPixels(Pixel** pArr, Pixel** pArrCopy, int width, int height, int start, int end);

void swissCheese(Pixel** pArr, int width, int height);
#endif