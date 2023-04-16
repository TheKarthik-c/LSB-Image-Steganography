
/*
NAME         : KARTHIK C
DATE         : 30/03/2023
DESCRIPTION  : LSB Image Steganography
SAMPLE INPUT : -e beautiful.bmp secret.txt stego.bmp
               -d stego.bmp karthik
SAMPLE OUTPUT: karthik.txt
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])    //Main function, input is passing through the command line and collecting in this function;
{
	if(argc >= 2)                   //Checking the argument count should be greater than two;
	{

		if(strcmp(argv[1] , "-e") == 0) //Checking arg value 1 is equal to -e;
		{
			if(argc >= 4)                    //If yes, then check the argument count should be grater than 4;
			{

				printf("proceed\n");  //If yes proceed;
			}
			else                  //If no, printing error message and exit;
			{
				printf("Please pass the min 4 argument\n");
				return 0;
			}
		}
		else if ( strcmp(argv[1], "-d")	== 0)   //Comapring the argument 1 with -d;
		{
			if (argc >= 3)               //If yes, check argument count;
			{
				printf("proceed\n");
			}
			else
			{
				printf("Please pass the min 3 argument\n");
				return 0;
			}
		}
	}
	else
	{
		printf("Please pass the command line argument\n");  //argument count is less than 2 then pass error message and then exit;
		return 0;
	}

	
	if ( check_operation_type(argv) == e_encode )      //Check_operation function is called for encode process
	{
		printf("Selected encoding \n");

		EncodeInfo encInfo;                             //Structure data type variable

        if ( read_and_validate_encode_args(argv, &encInfo) == e_success)    //read_and_validate_encode_args function called
		{
			printf("read and validate is successfully completed \n ");

			if( do_encoding(&encInfo) == e_success )    //do_encoding function is called,Based on condition the output is printed
			{
				printf("Encoding is done successfully\n");
			}
			else
			{
				printf("Encoding is failed\n");
			}
		}
		else
		{
			printf("read and validate is failure \n");
		}
	}

	else if ( check_operation_type(argv) == e_decode)         //Check_operation_type function is calledfor decode process;
	{
		printf("Selected decoding \n");

		DecodeInfo decInfo;

		if ( read_and_validate_decode_args(argv, &decInfo) == e_success)
		{
			printf("Read and validate is successfully completed \n");

			if( do_decoding(&decInfo) == e_success)    //Do_decoding function is called 
			{
				printf("Decodingis done successfully \n");
			}
			else
			{
				printf("Decoding is failed\n");
			}
		}
		else
		{
			printf("Read and validate is failed\n");
		}
	}
	else
	{
		printf("Please pass the correct option -e or -d \n");
	}
}

OperationType check_operation_type(char *argv[])   //Defination for check_operation_type , checking the which operation should be perform;
{
	if( strcmp(argv[1] , "-e") == 0 )
	{
		return e_encode;
	}
	else if( strcmp(argv[1], "-d") == 0 )
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
	
}
