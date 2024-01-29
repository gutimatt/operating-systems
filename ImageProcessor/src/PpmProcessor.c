/**
* a ppm file class to handle different functions
*
* Completion time: 1
*
* @author Gutierrez
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "../headers/PpmProcessor.h"
#include "../headers/PixelProcessor.h"

void readPPMHeader(FILE* file, struct PPM_Header* header){
    fscanf(file, "%s %d %d %d",
           header->magicNumber,
           &header->width,
           &header->height,
           &header->maxColorVal);

    printf("magicNumber: %c%c\n", header->magicNumber[0], header->magicNumber[1]);
    printf("width: %d\n", header->width);
    printf("height: %d\n", header->height);
    printf("maxColor: %d\n", header->maxColorVal);
}

void writePPMHeader(FILE* file, struct PPM_Header* header) {
    fprintf(file, "%s %d %d %d",
           header->magicNumber,
           header->width,
           header->height,
           header->maxColorVal);

    printf("magicNumber: %c%c\n", header->magicNumber[0], header->magicNumber[1]);
    printf("width: %d\n", header->width);
    printf("height: %d\n", header->height);
    printf("maxColor: %d\n", header->maxColorVal);
}

void makePPMHeader(struct PPM_Header* header, int width, int height){
    header->magicNumber[0] = 'P';
    header->magicNumber[1] = '6';
    header->width = width;
    header->height = height;
    header->maxColorVal = 255;
}

void readPixelsPPM(FILE* file, Pixel** pArr, int width, int height) {
    for (int i = 0; i < height; i++) {
        Pixel* row = (Pixel*) malloc(sizeof(Pixel) * width);
        for (int j = 0; j < width; j++) {
            fread(&row[j].r, sizeof(unsigned char), 1, file);
            fread(&row[j].g, sizeof(unsigned char), 1, file);
            fread(&row[j].b, sizeof(unsigned char), 1, file);
        }
        pArr[i] = row;
    }
}

void writePixelsPPM(FILE* file, Pixel** pArr, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fwrite(&pArr[i][i].r, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][i].g, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][i].b, sizeof(unsigned char), 1, file);
        }
    }
}
