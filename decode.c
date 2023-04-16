#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include<string.h>

/* Function Definitions */

/*read and validate for decoding*/

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if( strcmp(strstr(argv[2] , "."), ".bmp") == 0 ) //Checking argument 2 is .bmp file or not;
	{
		decInfo->stego_image_fname = argv[2];
	}
	else
	{
		return e_failure;
	}
	if( argv[3] != NULL)             //checking argument 3 file is present or not;
	{
		decInfo->output_name = argv[3];
	}
	else
	{
		decInfo->output_name = "default_output_name";
	}
	return e_success;
}

/*do decoding*/

Status do_decoding(DecodeInfo *decInfo)     //do_decodig fuction defination, inside this there are many sub functions;
{

	if( files_open(decInfo) == e_success )   //open file function call, to open the file in read mode
	{	
		printf("Open files successfully\n");
	}
	else
	{
		printf("Can't able to open the file\n");
		return e_failure;
	}

	if(decode_magic_string(decInfo,MAGIC_STRING) == e_success) //decode magic string from stego file;
	{
		printf("Successfully decoded magic string:\n");
	}
	else
	{
		printf("Sorry unable to decode magic string\n");
		return e_failure;
	}

	if( decInfo->sec_extn_size = decode_secfile_extn_size(decInfo)) //decode secret file extention size(int);
	{
		if(decInfo->sec_extn_size)
		{
			printf("Secret file extension size is %d\n",decInfo->sec_extn_size);
		}
		else
		{
			printf("cant find secret file extension size\n");
			return e_failure;
		}
	}

	decInfo->sec_extn[decInfo->sec_extn_size] = 0;

	if( decode_sec_file_extn (decInfo,decInfo->sec_extn_size) == e_success) //decode secret file extention size;
	{
	
		if(strlen(decInfo->secret_extension) == decInfo->sec_extn_size)
		{
			printf("Successfully decoded secret file extension\n");
		}
		else
		{
			printf("Unable to decode secret file extension\n");
			return e_failure;
		}
	}

	decInfo->arr[20];

	strcpy(decInfo->arr,decInfo->output_name);  //copying output name to array;

	strcat(decInfo->arr,decInfo->secret_extension); //cancatenaton of two string name and extention

	printf("%s\n",decInfo->arr);

	if(open_output_file(decInfo) == e_success)
	{
		printf("Successfully opened output file\n");
	}
	
	if( decode_sec_file_size(decInfo) == e_success) //decode secret file size from stego file
	{
		printf("Successfully decoded secret file size and it's size is %u\n",decInfo->sec_file_size);
	}
	if( decode_sec_file(decInfo, decInfo->sec_file_size) == e_success) //decode secret file 
	{
		printf("Successfull decode secret file data\n");
	}

	return e_success;		
}



//Decoding functions defination

Status decode_sec_file(DecodeInfo *decInfo , int size) //decode secret file from stego file based on size*8 ftech byte from that file;
{
	int i=0;
	decode_sec_file_extn(decInfo, size); //call decode secret file extention

	while(decInfo->secret_extension[i] != '\0')
	{
		fputc(decInfo->secret_extension[i], decInfo->fptr_output);
		i++;
	}

	return e_success;
}

Status decode_sec_file_size(DecodeInfo *decInfo) //decode sec file size
{
	if (decInfo->sec_file_size = decode_secfile_extn_size(decInfo)) //call decode secret file extnsize;
	{
		return e_success;	
	}
}

Status decode_sec_file_extn(DecodeInfo *decInfo,int size) //sec file exn definatio
{
	decInfo->secret_extension;

	char arr[10000] = {0};
	int i, j;
	char str[size][8];

	for(i = 0; i < size; i++)
	{
		fread(str[i], 1, 8, decInfo->fptr_stego_image);	//fetching byte based on size;

		for(j = 0; j < 8; j++)
		{

			arr[i] = (((str[i][j] & 1) << (7-j)) | arr[i]); //taking lsb bit and storing it in array;
		}
	}
	strcpy(decInfo->secret_extension, arr);

	if(decInfo->secret_extension != NULL)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
	

}

Status decode_secfile_extn_size(DecodeInfo *decInfo) //decode sec file extn size definatio
{
	int a=0, i;
	char image_buffer[32];

	fread(image_buffer ,1, 32, decInfo->fptr_stego_image); //fetch 32 byte from stego file

	for(i = 0; i < 32; i++)
	{
		a = ((image_buffer[i] & 1) << (31-i)) | a;  //get lsb bit and store ina avriable as a msb bit
		
	}
	return a;

}

Status decode_magic_string(DecodeInfo *decInfo, char *ch) //decode magic string
{
	int i = 0,j = 0;
	char str[2][8] = {0};
	char data[2] = {0};

	fseek(decInfo->fptr_stego_image, 54, SEEK_SET); //set pointer after 54 byte

	for(i = 0; ch[i] != '\0'; i++)
	{
		fread(str[i], 1, 8, decInfo->fptr_stego_image); //fetch 16 byte , 8 byte at a time;

		for(j = 0; j < 8; j++)
		{
			data[i] = (((str[i][j] & 1) << (7-j)) | data[i]); //get lsb bit store in array that bit in a msb place;
		}

		printf("%c\n",data[i]);
	}
	if(!strcmp(data, ch))
	{
		printf("MAGIC STRING is matched\n");
		return e_success;
	}
	else
	{
		printf("MAGIC STRING is not matched\n");
		return e_failure;
	}

}

Status files_open(DecodeInfo *decInfo) //file open defination
{
	//Stego file in decode operation
	decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r"); //open file in read mode

    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
	}
	return e_success;
}

Status open_output_file(DecodeInfo *decInfo)
{
	decInfo->fptr_output = fopen(decInfo->arr, "w"); //open output file in write mode

	if(decInfo->fptr_output == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->output_name);
	}
	return e_success;
}

