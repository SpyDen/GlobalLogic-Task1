#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "overlay.h"

void Overlay(void)
{
	FILE* file = NULL;
	FILE* csv1 = NULL;
	FILE* csv2 = NULL;
	FILE* csv = NULL;

	char path1[200], path2[200], path[200], path3[200];
	uint16_t seconds1, seconds2, sec1, sec2;
	uint32_t buffer_size;
	const uint16_t sampling_rate = 44100;
	uint16_t* buffer = NULL;
	double phase;

	printf("\033[32mPlease, enter the path to the first csv file: ");
	scanf("%s", path1);
	getchar();

	if ((csv1 = fopen(path1, "r")) == NULL)
	{
		puts("\033[31mSorry, it is undefined error!");
		exit(EXIT_FAILURE);
	}
	fseek(csv1, 0, SEEK_SET);

	printf("\033[32mPlease, enter the path to the second csv file: ");
	scanf("%s", path2);
	getchar();

	if ((csv2 = fopen(path2, "r")) == NULL)
	{
		puts("\033[31mSorry, it is undefined error!");
		exit(EXIT_FAILURE);
	}
	fseek(csv2, 0, SEEK_SET);

	for (uint8_t i = 0; i < 4; ++i)
		fgetc(csv1);

	fscanf(csv1, "%" SCNu16, &seconds1);

	for (uint8_t i = 0; i < 4; ++i)
		fgetc(csv2);

	fscanf(csv2, "%" SCNu16, &seconds2);

	do
	{
		printf("\033[32mPlease, enter the diapason of overlay:\n");
		printf("From: ");
		check_int_data(&sec1);
		printf("To: ");
		check_int_data(&sec2);

		if (sec1 >= sec2 || sec1 >= seconds1 || sec2 >= seconds1 || sec1 >= seconds2 || sec2 >= seconds2)
			{	
				printf("\033[31mYou have entered incorrect data! The diapason is overflow!\n");
			}
		else
			break;

	} while (1);

	if (seconds1 > seconds2)
		buffer_size = sampling_rate * seconds1;
	else
		buffer_size = sampling_rate * seconds2;

	uint32_t begin = sampling_rate * sec1;
	uint32_t end = sampling_rate * sec2;


	printf("\033[32mPlease, enter a path to the csv file will be created: ");
	scanf("%s", path3);
	getchar();

	if ((csv = fopen(path3, "w")) == NULL)
	{
		printf("\033[31mSorry, it is undefined error.\n");
		exit(EXIT_FAILURE);
	}

	fputc(' ', csv);
	fputc(',', csv);
	fputc(' ', csv);
	fputc(',', csv);
	fprintf(csv, "%" PRIu16, buffer_size / sampling_rate);
	fputc('\n', csv);

	if ((buffer = (uint16_t*)malloc(sizeof(uint16_t) * buffer_size)) == NULL) // If memory was not alloced
	{
		puts("\033[31mSory, but OS couldn\'t share a dynamic memory!");
		exit(EXIT_FAILURE);
	}

	for (register uint32_t i = 0; i < begin; i++)
	{
		fscanf(csv1, "%lf", &phase);
		fgetc(csv1);
		fscanf(csv1, "%" SCNu16, &buffer[i]); // Just for сatch up


		fscanf(csv2, "%lf", &phase);
		fgetc(csv2);
		fscanf(csv2, "%" SCNu16, &buffer[i]);
		fprintf(csv, "%lf", phase);
		fputc(',', csv);
		fprintf(csv, "%" PRIu16, buffer[i]);
		fputc('\n', csv);
	}

	uint16_t temp_buffer;
	double temp_phase;
	for (register uint32_t i = begin; i < end; i++)
	{
		fscanf(csv2, "%lf", &phase);
		fgetc(csv2);
		fscanf(csv2, "%" SCNu16, &buffer[i]);

		fscanf(csv1, "%lf", &temp_phase);
		fgetc(csv2);
		fscanf(csv2, "%" SCNu16, &temp_buffer);

		phase += temp_phase;
		buffer[i] += temp_buffer;
		fprintf(csv, "%lf", phase);
		fputc(',', csv);
		fprintf(csv, "%" PRIu16, buffer[i]);
		fputc('\n', csv);
	}

	for (register uint32_t i = end; i < buffer_size; i++)
	{
		fscanf(csv2, "%lf", &phase);
		fgetc(csv2);
		fscanf(csv2, "%" SCNu16, &buffer[i]);
		fprintf(csv, "%lf", phase);
		fputc(',', csv);
		fprintf(csv, "%" PRIu16, buffer[i]);
		fputc('\n', csv);
	}



	printf("\033[32mPlease, enter a path to the wav file will be created: ");
	scanf("%s", path);
	getchar();

	write_wav(path, buffer_size, buffer, sampling_rate);
	free(buffer); // free dynamic memory
	fclose(csv);
	fclose(csv1);
	fclose(csv2);
}

