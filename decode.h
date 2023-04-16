#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4


typedef struct _DecodeInfo
{
    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
	long extn_secret_file_size;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

	/*output file */
	char sec_extn[50];
	char arr[10000];
	uint sec_extn_size;
	uint sec_file_size;
	char *output_name;
	FILE *fptr_output;
	char secret_extension[1000000];
	

} DecodeInfo;

/*Decoding functions prototype*/

//read and validate of decode arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

//decode byte to data 
Status decode_byte_to_data(char *str);

//do_decoding function
Status do_decoding(DecodeInfo *decInfo);

//Decode magic string function
Status decode_magic_string(DecodeInfo *decInfo,char *ch);

//Files_open function
Status files_open(DecodeInfo *decInfo);

//secret file extn size (int) function
Status decode_secfile_extn_size(DecodeInfo *decInfo);

//decode secret file extn (char) functon
Status decode_sec_file_extn(DecodeInfo *decInfo,int size );

//decode secret file size (int) function
Status decode_sec_file_size(DecodeInfo *decInfo);

//decode secret file (char) function
Status decode_sec_file(DecodeInfo *decInfo , int size);

//to open output file function
Status open_output_file(DecodeInfo *decInfo);

#endif
