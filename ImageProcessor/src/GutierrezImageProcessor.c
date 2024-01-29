
/**
* main driver to handle cli cammands
*
* Completion time: 10
*
* @author Gutierrez, Acuna
* @version 1.0
*
*/

////////////////////////////////////////////////////////////////////////////////
// INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../headers/BmpProcessor.h"
#include "../headers/PpmProcessor.h"

void writeOutBMP(FILE* outputFile, Pixel **pxArr, DIB_Header *dibHeader, int bShift, int gShift, int rShift) {

        BMP_Header *bmpHeaderConvert = (BMP_Header *) malloc(sizeof(BMP_Header));
        DIB_Header *dibHeaderConvert = (DIB_Header *) malloc(sizeof(DIB_Header));

        makeBMPHeader(bmpHeaderConvert, dibHeader->width, dibHeader->height);
        writeBMPHeader(outputFile, bmpHeaderConvert);

        makeDIBHeader(dibHeaderConvert, dibHeader->width, dibHeader->height);
        writeDIBHeader(outputFile, dibHeaderConvert);

        colorShiftPixels(pxArr, dibHeader->width, dibHeader->height, rShift, gShift, bShift);

        writePixelsBMP(outputFile, pxArr, dibHeader->width, dibHeader->height);

        free(dibHeaderConvert);
        free(bmpHeaderConvert);
}

void writeOutPPM(FILE* outputFile, Pixel** pxArr, PPM_Header*  ppmHeader, int bShift, int gShift, int rShift) {
    PPM_Header *ppmHeaderConvert = (PPM_Header *) malloc(sizeof(PPM_Header));

    makePPMHeader(ppmHeaderConvert, ppmHeader->width, ppmHeader->height);
    writePPMHeader(outputFile, ppmHeaderConvert);

    colorShiftPixels(pxArr, ppmHeader->width, ppmHeader->height, rShift, gShift, bShift);

    writePixelsPPM(outputFile, pxArr, ppmHeader->width, ppmHeader->height);

    free(ppmHeaderConvert);
}

int main(int argc, char* argv[]) {
    if (argv[1] == NULL){
        printf("No data file name provided. This should be the first arg\n");
        exit(1);
    }

    char* gOffset = NULL;
    char* bOffset = NULL;
    char* rOffset = NULL;
    char* outfile;
    char* type;
    int opt;

    while((opt = getopt(argc, argv, ":o:r:g:b:t:")) != -1){
        switch(opt){
            case 'o':
                outfile = optarg;
                break;
            case 'r':
                rOffset = optarg;
                break;
            case 'g':
                gOffset = optarg;
                break;
            case 'b':
                bOffset = optarg;
                break;
            case 't':
                type = optarg;
                break;
            case ':':
                printf("option needs a value\n");
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }
    char* datafile = argv[optind];

    for (; optind< argc-1; optind++){
        printf("Extra arguments given %s\n", argv[optind+1]);
    }


    int bShift = 0;
    int gShift = 0;
    int rShift = 0;
    if (bOffset != NULL && strlen(bOffset) > 0) {
        bShift = (int) strtol(bOffset, NULL, 10);
    }
    if (gOffset != NULL && strlen(gOffset) > 0) {
        gShift = (int) strtol(gOffset, NULL, 10);
    }
    if (rOffset != NULL && strlen(rOffset) > 0) {
        rShift = (int) strtol(rOffset, NULL, 10);
    }

    int dflen;
    dflen = strlen(datafile);
    if(dflen >= 5 && (access(datafile, F_OK) != -1)){
        printf("Importing data from %s\n\n", datafile);
        FILE *inputFile = NULL;
        if (strcmp(&datafile[dflen-4], ".bmp") == 0) {
            inputFile = fopen(datafile, "rb+");

            BMP_Header* bmpHeader = (BMP_Header*)malloc(sizeof(BMP_Header));
            DIB_Header* dibHeader = (DIB_Header*)malloc(sizeof(DIB_Header));

            readBMPHeader(inputFile, bmpHeader);
            readDIBHeader(inputFile, dibHeader);

            Pixel** pxArr = (Pixel**) malloc(sizeof(int) * dibHeader->height);

            readPixelsBMP(inputFile, pxArr, dibHeader->width, dibHeader->height);

            int oflen;
            oflen = strlen(outfile);
            if (outfile != NULL && oflen >= 5){
                if(strcmp(&outfile[oflen-4], ".bmp") == 0) {
                    printf("\nExporting to %s\n", outfile);
                    FILE *outputFile = fopen(outfile, "wb");
                    writeOutBMP(outputFile, pxArr, dibHeader, bShift, gShift, rShift);
                    fclose(outputFile);
                }else {
                    printf("Invalid output name");
                    exit(1);
                }
            } else{
                writeOutBMP(inputFile, pxArr, dibHeader, bShift, gShift, rShift);
            }


            for (int i = 0; i < dibHeader->height; ++i) {
                free(pxArr[i]);
                pxArr[i] = NULL;
            }

            free(pxArr);

            free(dibHeader);
            free(bmpHeader);
            fclose(inputFile);

        } else if (strcmp(&datafile[dflen-4], ".ppm") == 0) {
            inputFile = fopen(datafile, "r+");

            PPM_Header* ppmHeader = (PPM_Header*) malloc(sizeof(PPM_Header));

            readPPMHeader(inputFile, ppmHeader);
            Pixel** pxArr = (Pixel**) malloc(sizeof(int) * ppmHeader->height);

            readPixelsPPM(inputFile, pxArr, ppmHeader->width, ppmHeader->height);

            int oflen;
            oflen = strlen(outfile);
            if (outfile != NULL && oflen >= 5){
                if(strcmp(&outfile[oflen-4], ".ppm") == 0) {
                    printf("\nExporting to %s\n", outfile);
                    FILE *outputFile = fopen(outfile, "w");
                    writeOutPPM(outputFile, pxArr, ppmHeader, bShift, gShift, rShift);
                    fclose(outputFile);
                }else {
                    printf("Invalid output name");
                    exit(1);
                }
            } else{
                writeOutPPM(inputFile, pxArr, ppmHeader, bShift, gShift, rShift);
            }


            for (int i = 0; i < ppmHeader->height; ++i) {
                free(pxArr[i]);
            }
            free(ppmHeader);
        }
        fclose(inputFile);
    }
    else {
        printf("Data file has an invalid name or does not exist.\n");
        exit(1);
    }
}