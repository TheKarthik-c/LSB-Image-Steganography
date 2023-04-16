#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<string.h>

/* Function Definitions */

//Read and validate for encoding;

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)  //read and validate function wheather passed argument is correct or not;
{
	if(strcmp(strstr(argv[2] , "."), ".bmp")==0)
	{
		encInfo -> src_image_fname = argv[2]; //Storing argument 2 into the structure member;
	}
	else
	{
		return e_failure;
	}
	if( argv[3] != NULL )
	{
		char *ptr;
		if( (ptr = strchr( argv[3] , '.' )) != NULL)   //extractng from . and storing into another char array;
		{
			strcpy( encInfo -> extn_secret_file, ptr);
			encInfo -> secret_fname = argv[3];   //argument 3 is storing into structure member;
		}
		else
		{
			return e_failure;
		}
	}
	else
	{
		return e_failure;
	}
	if( argv[4] != NULL)        //If argument 4 is passed, then store that name into some structure member;
	{
		char *ptr1=NULL;
		if( (ptr1 = strchr( argv[4] , '.')) != NULL)
		{
			if( strcmp(ptr1, ".bmp") == 0 )
			{
				encInfo -> stego_image_fname = argv[4];
			}
			else
			{
				printf("Please pass the argument 4 in .bmp\n");
			}
		}
		else
		{
			printf("Please pass the argument 4 in .bmp file\n");
			return e_failure;
		}
	}
	else                         //else take some default value;
	{
		encInfo -> stego_image_fname = "Default_stego.bmp";

	}
	return e_success;
}

/* do encoding */

Status do_encoding(EncodeInfo *encInfo)  //Do_encoding is the function defination, inside this function there are many sub function;
{
	if( open_files(encInfo) == e_success)
 	{
		printf("Open files successfully\n");
	}
	else
	{
		printf("Open files failure\n");
		return e_failure;
	}
	if( check_capacity(encInfo) == e_success ) //Check capacity function, can we able to hide the data if the space is there or not;
	{
		printf("check capacity is successfull done \n");
	}
	else
	{
		printf("check capacity is failed \n");
		return e_failure;
	}

	if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success ) //Copy header function, copying all 54 bytes to another file;
	{
		printf("succesfully bmp header is copied\n");
	}
	else
	{
		printf("uncopied\n");
		return e_failure;
	}
	
	if(encode_magic_string(MAGIC_STRING,encInfo) == e_success) //Magic string is encoded function
	{
		printf("Successfully encoded magic string\n");
		char ch[4];
		strcpy(ch, encInfo->extn_secret_file);

		if(encode_secret_file_extn_size(strlen(ch), encInfo) == e_success) //encode secrete file extension size;
		{
			printf("Successfully encoded secret file extension size\n");
			strcpy(encInfo->extn_secret_file,ch);

			if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success) //encode secret file extension
			{
				printf("Secret file extension is encoded succesfully\n");

				if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success) //encode secret file size (int);
				{
					printf("secrest file size is encoded\n");
				 
					if(encode_secret_file_data(encInfo) == e_success)  //encode secret file data
					{
						printf("Successfully encoded secret file character\n");

						if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success) //reamining byte in beautiful.bmp is to be copied and paste in to the another file called stego file;
						{
							printf("Suceesfully copied remaining data from source file to stego file\n");

						}
					
					}
				
				}
			
			}
		
		}
	
	}
	
	return e_success;

}

//Encoding functions defination;

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest) 
{
	unsigned int i = ftell(fptr_src); //fing out the current pointer
	fseek(fptr_src, 0, SEEK_END);       //logic is used to copy reamining byte,

	unsigned int j = ftell(fptr_src); //find out the last pointer
	fseek(fptr_src, i, SEEK_SET);

	char str[j-i];
	fread(str, 1, j-i, fptr_src); //fetch the data from beautiful.bmp
	fwrite(str, 1, j-i, fptr_dest);//store the data to output file;

	return e_success;

}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char c;
	int i;
	char str[encInfo->size_secret_file + 1];

	str[encInfo->size_secret_file] = 0;

	fread(str, 1, encInfo->size_secret_file, encInfo->fptr_secret);

	for(i=0; str[i]!='\0'; i++)
	{
		fread(encInfo->image_data,1,8,encInfo->fptr_src_image);  //read the byte from source file, based on size of secret file;

		encode_byte_to_lsb(str[i], encInfo->image_data);   //call the function as encode_bute_to_lsb , to perform hide data operation

		fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image); //after performing store into output file; 
	}

	return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)  
{
	if ((encode_secret_file_extn_size(file_size, encInfo))==e_success)//call the function , to convert integer to a bit and storing into another file;
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   	int i;

	for(i=0; i<strlen(file_extn); i++)               //if extension lengthis 4, then for 1 byte fetch 8 byte of data from source file;
	{
		fread(encInfo->image_data,1,8,encInfo->fptr_src_image);

		encode_byte_to_lsb(file_extn[i], encInfo->image_data); //call the function byte to lsb to perform char to bite that bit is store into a i byte of lsb side;

		fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image);//after performing store that into output file
		
	}
	return e_success;

}


Status encode_secret_file_extn_size(long int size, EncodeInfo *encInfo) //for secret file extension size;
{

	char str[32];

	fread(str,1,32,encInfo->fptr_src_image);

	encode_int_to_lsb(size, str);  //call the function int_to_lsb.

	fwrite(str,1,32,encInfo->fptr_stego_image);

	return e_success;
}

Status encode_int_to_lsb(int size, char *image_buffer)  //this function is used to to convert value to bit; from msb take 1 bit and replace that bit to lsb side from souce byr;
{
	int i;
	for(i=0; i<32; i++)
	{
		image_buffer[i] = ((image_buffer[i] & ~1) |( ((1<<(31-i)) & size ) >> (31-i)));
	}
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo) //MAGIC STRING encode opeartion
{
	int i;

	fseek(encInfo->fptr_src_image,54,SEEK_SET);

	for(i=0; magic_string[i]!='\0'; i++)
	{
		fread(encInfo->image_data,1,8,encInfo->fptr_src_image); //For 1 byte take 8 byte of data from source file;

		encode_byte_to_lsb(magic_string[i], encInfo->image_data); //perfotm opeartion

		fwrite(encInfo->image_data,1,8,encInfo->fptr_stego_image);//after this, store that byte into output file;
	}

	return e_success;
}


Status encode_byte_to_lsb(char data, char *image_buffer) //take 1 bit from 1 byte; that 1 bit is to replace at 1 lsb bit from source file and store into some buffer
{
	int i;

	for(i=0; i<8; i++)
	{
		image_buffer[i] = ((image_buffer[i] & ~1) |( ((1<<(7-i)) & data ) >> (7-i)));
	}
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image) //Copying header byt from source filee  to output file
{
	char str[54];
	fseek(fptr_src_image,0, SEEK_SET);

	fread(str,1,54,fptr_src_image);

	fwrite(str,1,54,fptr_dest_image);

	fseek(fptr_dest_image,54,SEEK_SET);

	if(fptr_dest_image == NULL)
	{
		return e_failure;
	}
	else
	{
		return e_success;
	}
}

Status check_capacity(EncodeInfo *encInfo)
{
	encInfo->image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

	fseek(encInfo -> fptr_secret, 0, SEEK_END);

	encInfo->size_secret_file = ftell(encInfo->fptr_secret);

	rewind(encInfo->fptr_secret); //Check capacity that we can able to hide the data or not this file should be less than beautiful file 

	if ( ((strlen(MAGIC_STRING) + sizeof(int) + strlen(encInfo->extn_secret_file) + sizeof(int) + (encInfo->size_secret_file)) * 8) < encInfo->image_capacity)
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

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
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

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
    // Src Image file
	
    encInfo->fptr_src_image = fopen(encInfo -> src_image_fname, "r");

    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
