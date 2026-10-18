#ifndef DECODE_H
#define DECODE_H

#include<stdio.h>
#include "types.h"  // Contains user defined types


/* 
 * Structure to store information required for
 * Decoding secret file from stego Image
 * Info about secret and magic string is
 * also stored
 */

#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo{
     /* Stego Image info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char *output_fname;
    FILE *fptr_output;

    char extn_secret_file[MAX_FILE_SUFFIX];
    long size_secret_file;
}DecodeInfo;

/* Decoding function prototype */

/*Checking operation type*/
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_validate_decode_args(int argc,char *argv[],DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decoding magic string */
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo);

/*Decoding secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decoding secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decoding data from image */
Status decode_data_from_image(char *data,int size,FILE *fptr_stego_image);

/* Decoding byte from lsb */
char decode_byte_from_lsb(char *image_buffer);


#endif