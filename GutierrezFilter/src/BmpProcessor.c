/**
* a bmp file class basic functionality to convert
*
* Completion time: 10
*
* @author Gutierrez
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"


void readBMPHeader(FILE* file, struct BMP_Header* header){
    fread(&header->signature, sizeof(char), 2, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->reserved1, sizeof(short), 1, file);
    fread(&header->reserved2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);

    printf("READING BMP HEADER DATA\n");

    printf("signature: %c%c\n", header->signature[0], header->signature[1]);
    printf("size: %d\n", header->size);
    printf("reserved1: %d\n", header->reserved1);
    printf("reserved2: %d\n", header->reserved2);
    printf("offset_pixel_array: %d\n\n", header->offset_pixel_array);
}


void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(&header->signature, sizeof(char)*2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);

    printf("WRITE BMP HEADER DATA\n");

    printf("signature: %c%c\n", header->signature[0], header->signature[1]);
    printf("size: %d\n", header->size);
    printf("reserved1: %d\n", header->reserved1);
    printf("reserved2: %d\n", header->reserved2);
    printf("offset_pixel_array: %d\n\n", header->offset_pixel_array);
}

void readDIBHeader(FILE* file, struct DIB_Header* header) {

    fread(&header->size, sizeof(int), 1, file);
    fread(&header->width, sizeof(int), 1, file);
    fread(&header->height, sizeof(int), 1, file);
    fread(&header->planes, sizeof(short), 1, file);
    fread(&header->bitsPerPixel, sizeof(short), 1, file);
    fread(&header->compression, sizeof(int), 1, file);
    fread(&header->imageSize, sizeof(int), 1, file);
    fread(&header->horizRes, sizeof(int), 1, file);
    fread(&header->vertRes, sizeof(int), 1, file);
    fread(&header->colorNum, sizeof(int), 1, file);
    fread(&header->importantColorNum, sizeof(int), 1, file);

    printf("READING DIB HEADER DATA\n");

    printf("size: %d\n", header->size);
    printf("width: %d\n", header->width);
    printf("height: %d\n", header->height);
    printf("planes: %d\n", header->planes);
    printf("bitsperpixel: %d\n", header->bitsPerPixel);
    printf("compression: %d\n", header->compression);
    printf("imagesize: %d\n", header->imageSize);
    printf("x pixel per meter: %d\n", header->horizRes);
    printf("y pixel per meter: %d\n", header->vertRes);
    printf("colorNum: %d\n", header->colorNum);
    printf("numImportColor: %d\n", header->importantColorNum);

}

void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(int), 1, file);
    fwrite(&header->height, sizeof(int), 1, file);
    fwrite(&header->planes, sizeof(short), 1, file);
    fwrite(&header->bitsPerPixel, sizeof(short), 1, file);
    fwrite(&header->compression, sizeof(int), 1, file);
    fwrite(&header->imageSize, sizeof(int), 1, file);
    fwrite(&header->horizRes, sizeof(int), 1, file);
    fwrite(&header->vertRes, sizeof(int), 1, file);
    fwrite(&header->colorNum, sizeof(int), 1, file);
    fwrite(&header->importantColorNum, sizeof(int), 1, file);

    printf("WRITING DIB HEADER DATA\n");

    printf("size: %d\n", header->size);
    printf("width: %d\n", header->width);
    printf("height: %d\n", header->height);
    printf("planes: %d\n", header->planes);
    printf("bitsperpixel: %d\n", header->bitsPerPixel);
    printf("compression: %d\n", header->compression);
    printf("imagesize: %d\n", header->imageSize);
    printf("x pixel per meter: %d\n", header->horizRes);
    printf("y pixel per meter: %d\n", header->vertRes);
    printf("colorNum: %d\n", header->colorNum);
    printf("numImportColor: %d\n", header->importantColorNum);
}

void makeBMPHeader(struct BMP_Header* header, int width, int height){
    header->signature[0] = 'B';
    header->signature[1] = 'M';
    int size = 54 + (width*height) * 3 + ((4 - (width * 3 % 4)) % 4) * height;
    header->size = size;
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset_pixel_array = 54;
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    header->size = 40;
    header->width = width;
    header->height = height;
    header->planes = 1;
    header->bitsPerPixel = 24;
    header->compression = 0;
    header->imageSize = (width*height) * 3 + ((4 - (width * 3 % 4)) % 4) * height;
    double meterPerPx = 0.0002645833;
//    header->horizRes = (int) (width * meterPerPx);
//    header->vertRes = (int) (height * meterPerPx);
    header->horizRes = 3780;
    header->vertRes = 3780;
    header->colorNum = 0;
    header->importantColorNum = 0;
}

void readPixelsBMP(FILE* file, Pixel** pArr, int width, int height) {
    int padding = (4 - ((width*3) % 4)) % 4;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fread(&pArr[i][j].blue, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].green, sizeof(unsigned char), 1, file);
            fread(&pArr[i][j].red, sizeof(unsigned char), 1, file);
        }
        fseek(file, padding, SEEK_CUR);
    }
}


void writePixelsBMP(FILE* file, Pixel** pArr, int width, int height) {
    int padding = (4 - ((width*3) % 4)) % 4;
    unsigned char paddingVal = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            fwrite(&pArr[i][j].blue, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].green, sizeof(unsigned char), 1, file);
            fwrite(&pArr[i][j].red, sizeof(unsigned char), 1, file);

        }
        fwrite(&paddingVal, sizeof(unsigned char), padding, file);
    }
}