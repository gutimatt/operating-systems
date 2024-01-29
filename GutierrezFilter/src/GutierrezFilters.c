/**
* main file for program.  Allows to take in input output name and type of filter.
 * Can blur the image using threads or make the image look like swiss cheese.
*
* Completion time: 10
*
* @author Gutierrez
* @version 2.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "BmpProcessor.h"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096
#define THREAD_COUNT 4

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

typedef enum {blur, cheese}f_type;

//pxArr, pxArrCopy, dibHeader->width, dibHeader->height
typedef struct {
    Pixel** pxArray;
    Pixel** pxCopy;
    int start;
    int end;
    int width;
    int height;
}BlurRunnerData;

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

void* blurRunner(void* param){
    BlurRunnerData* data = (BlurRunnerData*)param;
    blurPixels(data->pxArray, data->pxCopy, data->width, data->height, data->start, data->end);
    pthread_exit(0);
}

void writeOutBMP(FILE* outputFile, Pixel **pxArr, DIB_Header *dibHeader, f_type ft) {

    BMP_Header *bmpHeaderConvert = (BMP_Header *) malloc(sizeof(BMP_Header));
    DIB_Header *dibHeaderConvert = (DIB_Header *) malloc(sizeof(DIB_Header));

    makeBMPHeader(bmpHeaderConvert, dibHeader->width, dibHeader->height);
    writeBMPHeader(outputFile, bmpHeaderConvert);

    makeDIBHeader(dibHeaderConvert, dibHeader->width, dibHeader->height);
    writeDIBHeader(outputFile, dibHeaderConvert);

    pthread_t tids[THREAD_COUNT];
    int subArrWidth = dibHeader->width / THREAD_COUNT;

    if(ft == blur) {
        BlurRunnerData** infos = (BlurRunnerData**) malloc(sizeof(BlurRunnerData*) * THREAD_COUNT);

        for (int i = 0; i < THREAD_COUNT; ++i) {
            infos[i] = (BlurRunnerData*)malloc(sizeof(BlurRunnerData));

            infos[i]->height = dibHeader->height;
            infos[i]->width = dibHeader->width;
            infos[i]->start = i * subArrWidth;
            infos[i]->end = (i+1) * subArrWidth;

            int padding = (4 - ((dibHeader->width * 3) % 4)) % 4;

            Pixel** pxArr_c = (Pixel**)malloc(sizeof(Pixel*) * dibHeader->height);
            for (int j = 0; j < dibHeader->height; ++j) {
                pxArr_c[j] = (Pixel*) malloc(sizeof(Pixel) * (subArrWidth + 1));
                for (int k = 0; k < subArrWidth + 1; ++k) {
                    pxArr_c[j][k].blue = pxArr[j][i * subArrWidth + k].blue;
                    pxArr_c[j][k].green = pxArr[j][i * subArrWidth + k].green;
                    pxArr_c[j][k].red = pxArr[j][i * subArrWidth + k].red;
                }
            }

            infos[i]->pxArray = pxArr_c;

            Pixel** pxArrCopy = (Pixel**)malloc(sizeof(Pixel*) * dibHeader->height);
            for (int j = 0; j < dibHeader->height; ++j) {
                pxArrCopy[j] = (Pixel*) malloc(sizeof(Pixel) * subArrWidth);
            }

            infos[i]->pxCopy = pxArrCopy;

            pthread_create(&tids[i], NULL, blurRunner, infos[i]);
        }

        for (int i = 0; i < THREAD_COUNT; ++i) {
            pthread_join(tids[i], NULL);
        }

        for (int j = 0; j < THREAD_COUNT; ++j) {
            for (int i = 0; i < dibHeader->height; ++i) {
                BlurRunnerData* info = infos[j];
                for (int k = 0; k < subArrWidth + 1; ++k) {
                    pxArr[i][subArrWidth * j + k].blue = info->pxCopy[i][k].blue;
                    pxArr[i][subArrWidth * j + k].green = info->pxCopy[i][k].green;
                    pxArr[i][subArrWidth * j + k].red = info->pxCopy[i][k].red;
                }
            }
        }

        writePixelsBMP(outputFile, pxArr, dibHeader->width, dibHeader->height);

        for (int i = 0; i < THREAD_COUNT; ++i) {
            for (int j = 0; j < dibHeader->height; ++j) {
                free(infos[i]->pxCopy[j]);
                free(infos[i]->pxArray[j]);
            }
            free(infos[i]->pxCopy);
            free(infos[i]->pxArray);

            free(infos[i]);
        }

        free(infos);

    } else {
        colorShiftPixels(pxArr, dibHeader->width, dibHeader->height, 0, 0, -100);
        swissCheese(pxArr, dibHeader->width, dibHeader->height);
        writePixelsBMP(outputFile, pxArr, dibHeader->width, dibHeader->height);
    }


    free(dibHeaderConvert);
    free(bmpHeaderConvert);
}

void main(int argc, char* argv[]) {
    char* infile;
    char* outfile;
    char* filter_t;
    int opt;

    while((opt = getopt(argc, argv, ":i:o:f:")) != -1){
        switch(opt){
            case 'i':
                infile = optarg;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'f':
                filter_t = optarg;
                break;
        }
    }

    for (; optind< argc; optind++){
        printf("Extra arguments given %s\n", argv[optind]);
    }

    if (filter_t[0] != 'b' && filter_t[0] != 'c'){
        printf("invalid filter.\n");
        exit(1);
    }

    int dflen;
    dflen = strlen(infile);
    if(dflen >= 5 && (access(infile, F_OK) != -1) &&
            strcmp(&infile[dflen-4], ".bmp") == 0){

        printf("Importing data from %s\n\n", infile);
        FILE *inputFile = NULL;
        inputFile = fopen(infile, "rb+");


        BMP_Header* bmpHeader = NULL;
        DIB_Header* dibHeader = NULL;

        bmpHeader = (BMP_Header*)malloc(sizeof(BMP_Header));
        dibHeader = (DIB_Header*)malloc(sizeof(DIB_Header));

        readBMPHeader(inputFile, bmpHeader);
        readDIBHeader(inputFile, dibHeader);

        Pixel** pxArr = (Pixel**) malloc(sizeof(Pixel*) * dibHeader->height);

        for (int i = 0; i < dibHeader->height; i++) {
            pxArr[i] = (Pixel*) malloc(sizeof(Pixel) * dibHeader->width);
        }

        readPixelsBMP(inputFile, pxArr, dibHeader->width, dibHeader->height);

        int oflen;
        oflen = strlen(outfile);
        if (outfile != NULL && oflen >= 5 &&
            strcmp(&outfile[oflen-4], ".bmp") == 0) {
                printf("\nExporting to %s\n", outfile);
                FILE *outputFile = fopen(outfile, "wb");
                f_type ft = filter_t[0] == 'b' ? blur : cheese;
                writeOutBMP(outputFile, pxArr, dibHeader, ft);
                fclose(outputFile);
        } else {
            printf("Invalid output file\n");
        }

        for (int i = 0; i < dibHeader->height; i++) {
            free(pxArr[i]);
            pxArr[i] = NULL;
        }

        free(pxArr);

        free(dibHeader);
        free(bmpHeader);
        fclose(inputFile);
    }
    else {
        printf("Data file has an invalid name or does not exist.\n");
        exit(1);
    }
}