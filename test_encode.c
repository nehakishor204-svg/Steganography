/* NAME : NEHA KISHOR
   DATE : 06/10/2026 
   Main executing part */
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    OperationType operation;
    if(argc<2){
        printf("Usage:\n");
        printf("Encoding: %s -e source.bmp secret.txt output.bmp\n",argv[0]);
        printf("Decoding: %s -d stego.bmp [output.txt]\n",argv[0]);
        return 1;
    }
    operation = check_operation_type(argv);

    /* Checking if operation is encode */
    if(operation == e_encode){
        if(argc != 5){
            printf("Usage: %s -e source.bmp secret.txt output.bmp\n",argv[0]);
            return 1;
        }
        EncodeInfo encInfo;
    
    
        if(read_and_validate_encode_args(argv,&encInfo) == e_failure){
            return 1;
        }
        if(do_encoding(&encInfo) == e_failure){
            printf("ERROR: Encoding failed\n");
            return 1;
        }
        
    }

    /* Checking  if operation is decoding */
    else if(operation == e_decode){
        if(argc != 3 && argc != 4){
            printf("Usage: %s -d stego.bmp [output.txt]\n",argv[0]);
            return 1;
        }
        DecodeInfo decInfo;
        if(read_validate_decode_args(argc,argv,&decInfo) == e_failure){
            return 1;
        }
        if(do_decoding(&decInfo) == e_failure){
            printf("ERROR: Decoding failed\n");
            return 1;
        }
    }
    else{
        printf("Unsupported operation\n");
        return 1;
    }
    return 0;
}