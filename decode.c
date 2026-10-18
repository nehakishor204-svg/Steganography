/* NAME : NEHA KISHOR 
   DATE : 06/10/2026
   STEGANOGRAPHY
   Decoding stego image , magic string , secret data */
#include<stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/*Read and validate decode arguments*/
Status read_validate_decode_args(int argc,char *argv[],DecodeInfo *decInfo){
    decInfo -> stego_image_fname = argv[2];
    if(argc == 4){
        decInfo -> output_fname = argv[3];
    }
    else{
       decInfo -> output_fname = NULL; 
    }
    return e_success;
}

/*Opening required files for decoding*/
Status open_decode_files(DecodeInfo *decInfo){
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname,"rb");
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n",decInfo->stego_image_fname);
    
    return e_success;

}

/*Decoding byte from lsb using image_buffer*/
char decode_byte_from_lsb(char *image_buffer){
    char data = 0;
    for(int i=0;i<8;i++){
        data = (data << 1 ) | (image_buffer[i] & 1);
    }
    return data;
}

/* Decoding data from image */
Status decode_data_from_image(char *data,int size,FILE *fptr_stego_image){
    /* Setting image buffer as 8*/
    char image_buffer[8];
    for(int i=0;i<size;i++){
        if(fread(image_buffer,1,8,fptr_stego_image) != 8){
            return e_failure;
        }
        /*Storing bytes to data[i]*/
        data[i] = decode_byte_from_lsb(image_buffer);

    }
    return e_success;
}

/*Function for decoding magic string*/
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo){
    char decoded_magic[3];
    if(decode_data_from_image(decoded_magic,strlen(magic_string),decInfo->fptr_stego_image)==e_failure){
        return e_failure;
    }
    decoded_magic[strlen(magic_string)] = '\0';
    if(strcmp(decoded_magic,magic_string) == 0){
        return e_success;
   }
   return e_failure;
}

/* Function for decoding secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo){
    if(decode_data_from_image(decInfo->extn_secret_file,MAX_FILE_SUFFIX - 1,decInfo->fptr_stego_image) == e_failure){
        return e_failure;
    }
    decInfo->extn_secret_file[MAX_FILE_SUFFIX - 1] = '\0';
    return e_success;
}

/* Function for decoding secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo){
    long file_size;
    char *ptr = (char *)&file_size;
    if(decode_data_from_image(ptr,sizeof(file_size),decInfo->fptr_stego_image) == e_failure){
        return e_failure;
    }
    decInfo->size_secret_file = file_size;
    return e_success;
}

/* Function for Decoding secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo){
    char data[1];
    for(int i=0;i<decInfo->size_secret_file;i++){
        if(decode_data_from_image(data,1,decInfo->fptr_stego_image) == e_failure){
            return e_failure;
        }
        if(fwrite(data,1,1,decInfo->fptr_output) != 1){
            return e_failure;
        }
    }
    return e_success;
}

/* Function for Decoding by calling other function*/
Status do_decoding(DecodeInfo *decInfo){
    printf("INFO: ## Decoding Procedure Started ##\n\n");
    printf("INFO: Opening required files\n");

    /* Calling open decode files and failure checking */
    if(open_decode_files(decInfo) == e_failure){
        return e_failure;
    }
    printf("INFO: Done\n\n");
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    printf("INFO: Decoding Magic String Signature\n");

    /* Calling decode magic string and failure checking */
    if(decode_magic_string(MAGIC_STRING,decInfo) == e_failure){
        
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: Decoding Output File Extension\n");

    /* Calling decode file extension and failure checking */
    if(decode_secret_file_extn(decInfo) == e_failure){
       
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("INFO: Done\n\n");
    if(decInfo->output_fname == NULL){
        decInfo->output_fname = "decoded.txt";
        printf("INFO: Output File not mentioned. Creating decoded.txt as default\n");
    }

    /* Writing data in output file */
    decInfo->fptr_output = fopen(decInfo->output_fname,"wb");
    if(decInfo->fptr_output == NULL){
        perror("fopen");
        fprintf(stderr,"ERROR: Unable to open file %s\n",decInfo->output_fname);
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("INFO: Opened %s\n",decInfo->output_fname);
    printf("INFO: Done. Opened all required files\n\n");
    printf("INFO: Decoding File Size\n");

    /* Calling decode secret file size and failure checking */
    if(decode_secret_file_size(decInfo) == e_failure){
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: Decoding File Data\n");

    /* Calling decode file data and failure checking */
    if(decode_secret_file_data(decInfo) == e_failure){
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
        return e_failure;
    }
    printf("INFO: Done\n\n");

    /* Closing stego image and output file */
    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);
    printf("INFO: ## Decoding Done Successfully ##\n");
    return e_success;
}

/* Test decode using : ./a.out -d stego_img.bmp */


   


