#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint8_t  BYTE;

void MakeImages(FILE *inptr);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "You must supply only one argument\n");
        return 1;
    }
    FILE *inptr = fopen(argv[1], "r");
    FILE *outptr;
    if (inptr == NULL) {
        fprintf(stderr, "File supplied cannot be read\n");
    }
    BYTE buffer[512];
    int fileNum = fread(buffer, 1, 512, inptr);

    //skip over part of card that isn't jpeg
    while(buffer[0] != 0xff && buffer[1] != 0xd8 && buffer[2] != 0xff && (buffer[3] & 0xf0) != 0xe0) {

        fread(buffer,1, 512, inptr);
    }
    //initialize everything outside of while loop
    int fileCount = 0;
    char fileName[sizeof(char)*10];
    sprintf(fileName, "%03i.jpg", fileCount);
    printf("%s\n", fileName);
    outptr = fopen(fileName, "w");
    fwrite(buffer, 1, 512, outptr);
    fileNum = fread(buffer, 1, 512, inptr);
    //while we continue to read 512 byte chunks
    while(fileNum == 512){

        //Is Header?
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            fclose(outptr);
            fileCount++;
            sprintf(fileName, "%03i.jpg", fileCount);
            printf("%s\n", fileName); //debug
            outptr = fopen(fileName, "w");
        }
        fwrite(buffer, 1, 512, outptr);

        fileNum = fread(buffer, 1, 512, inptr);
    }

    fclose(outptr);
    fclose(inptr);

}