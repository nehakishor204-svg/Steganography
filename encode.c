/* NAME : NEHA KISHOR 
   DATE : 06/10/2026
   STEGANOGRAPHY
   Encoding source image , magic string , secret data*/
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */ 

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n",encInfo->src_image_fname);
    printf("INFO: Opened %s\n",encInfo->secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n",encInfo->stego_image_fname);
    printf("INFO: Done\n\n");

    // No failure return e_success
    return e_success;
}

/* Function to get file size
   pointing fseek 0th positon*/
long get_file_size(FILE *fptr){
    fseek(fptr,0,SEEK_END);
    long size = ftell(fptr);
    fseek(fptr,0,SEEK_SET);
    return size;
}

/* Function for checking capacity for storing secret data */
Status check_capacity(EncodeInfo *encInfo){
    uint required_capacity = (strlen(MAGIC_STRING) + strlen(encInfo->extn_secret_file) + sizeof(long) + encInfo->size_secret_file) *8;
    if(required_capacity <= encInfo->image_capacity){
        return e_success;
    }
    else{
        return e_failure;
    }
}

/* Function for copying bmp header from source image to destination image */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    char header[54];
    
    /*Reading header from source image */
    if(fread(header,1,54,fptr_src_image) != 54){
        return e_failure;
    }
    
    /* Writing image to destination image */
    if(fwrite(header,1,54,fptr_dest_image) != 54){
        return e_failure;
    }
    return e_success;
}

/* Function for encoding bytes to lsb*/
Status encode_byte_to_lsb(char data,char *image_buffer){
     
    for(int i=0;i<8;i++){
        int bit = (data>>(7-i))&1;
        image_buffer[i] = (image_buffer[i] & 0xFE) | bit;
    }
    return e_success;
}

/* Function for encoding data to image */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    char image_buffer[8];
    for(int i=0;i<size;i++){

        /* Reading image buffer for 8 bytes from source image */
        if(fread(image_buffer,1,8,fptr_src_image) != 8){
            return e_failure;
        }

        /* Calling encode_byte to lsb and failure checking */
        if(encode_byte_to_lsb(data[i],image_buffer) == e_failure){
            return e_failure;
        }

        /* Writing image buffer to stego image */
        if(fwrite(image_buffer,1,8,fptr_stego_image) != 8){
            return e_failure;
        }
    }
    return e_success;
}

/*Function for encoding magic string*/
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo){

    /*Calling encode data to image and failure checking*/
    if(encode_data_to_image((char*)magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure){
        return e_failure;
    }
    return e_success;
}

/* Function for encoding secret file extension */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo){

    /* Calling encode data to image */
    if(encode_data_to_image((char *)file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure){
        return e_failure;
    }
    return e_success;
}

/* Function encode secret file size */
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo){
    char *ptr = (char *)&file_size;

    /* Calling encoding data to image and failure checking */
    if(encode_data_to_image(ptr,sizeof(file_size),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_failure){
        return e_failure;
    }
    return e_success;
}

/* Function encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo){
    char secret_buffer[1];
    while(fread(secret_buffer, 1, 1, encInfo->fptr_secret) == 1){

        /*Calling encode data to image checking failure too*/
        if(encode_data_to_image(secret_buffer,1,encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure){
            return e_failure;
        }
       
    }
    return e_success;

}

/* Function for copying remaining image data to destination image */
Status copy_remaining_img_data(FILE *fptr_src,FILE *fptr_dest){
    char buffer[1024];
    int bytes_read;
    while((bytes_read = fread(buffer,1,1024,fptr_src)) > 0){
        if(fwrite(buffer,1,bytes_read,fptr_dest) != bytes_read){
            return e_failure;
        }
    }
    return e_success;
}

/* Calling whole functions to do encoding 
   output : returns e_success or e_ failure and printing info messages*/
Status do_encoding(EncodeInfo *encoInfo){
    if(open_files(encoInfo) == e_failure){
        return e_failure;
    }
    printf("INFO: ## Encoding Procedure Started ##\n\n");
    printf("INFO: Checking for %s size\n",encoInfo->secret_fname);
    encoInfo->image_capacity = get_image_size_for_bmp(encoInfo->fptr_src_image);
    encoInfo->size_secret_file = get_file_size(encoInfo->fptr_secret);
    fseek(encoInfo->fptr_src_image,0,SEEK_SET);
    if(encoInfo->size_secret_file > 0){
        printf("INFO: Done. Not Empty\n\n");
    }
    else{
        printf("ERROR: Secret file is empty\n");
        return e_failure;
    }
    printf("INFO: Checking for %s capacity to handle %s\n",encoInfo->src_image_fname,encoInfo->secret_fname);
    if(check_capacity(encoInfo) == e_failure){
        printf("ERROR: Insufficient image capacity\n");
        return e_failure;
    }
    printf("INFO: Done. Found OK\n\n");
    printf("INFO: Copying Image Header\n");
    if(copy_bmp_header(encoInfo->fptr_src_image,encoInfo->fptr_stego_image) == e_failure){
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: Encoding Magic String Signature\n");
    if(encode_magic_string(MAGIC_STRING,encoInfo) == e_failure){
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: Encoding %s File Extension\n",encoInfo->secret_fname);
    if(encode_secret_file_extn(encoInfo->extn_secret_file,encoInfo) == e_failure){
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: Encoding %s File Size\n",encoInfo->secret_fname);
    if(encode_secret_file_size(encoInfo->size_secret_file,encoInfo) == e_failure){
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: Encoding %s File Data\n",encoInfo->secret_fname);
    if(encode_secret_file_data(encoInfo) == e_failure){
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: Copying Left Over Data\n");
    if(copy_remaining_img_data(encoInfo->fptr_src_image,encoInfo->fptr_stego_image) == e_failure){
        return e_failure;
    }
    printf("INFO: Done\n\n");
    printf("INFO: ## Encoding Done Successfully ##\n");
    fclose(encoInfo->fptr_src_image);
    fclose(encoInfo->fptr_secret);
    fclose(encoInfo->fptr_stego_image);
    
    return e_success;
}
OperationType check_operation_type(char *argv[]){
    if(strcmp(argv[1],"-e") == 0){
        return e_encode;
    }
    if(strcmp(argv[1],"-d") == 0){
        return e_decode;
    }
    return e_unsupported;
}
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo){
    encInfo -> src_image_fname = argv[2];
    encInfo -> secret_fname = argv[3];
    encInfo -> stego_image_fname = argv[4];
    char *dot = strrchr(encInfo->secret_fname,'.');
    if(dot != NULL){
        strcpy(encInfo->extn_secret_file,dot);
    }
    return e_success;
}
/* Compile using : gcc test_encode.c encode.c decode.c -o a.out
   Run           : ./a.out -e beautiful.bmp secret.txt stego_img.bmp*/