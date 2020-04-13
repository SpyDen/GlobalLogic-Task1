#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include "overlay.h"
#include "creating.h"

void creating_sound(void)
{
	FILE* csv = NULL;
	double amplitude = UINT16_MAX; // The maximum value for 16-bit
	double frequency;
	double phase;
	const uint16_t sampling_rate = 44100; // Sampling rate for CD
	uint16_t seconds;
	uint32_t buffer_size;
	uint16_t* buffer = NULL;
	char path1[200]; // Path to the future wav file
	char path2[200]; // Path to the csv file
	char type; // The type of signal

	double frequency_radians_per_sample;

	printf("\033[32mPlease, enter the size of area in sec where will be created signal: ");
	check_int_data(&seconds);

	printf("\033[32mPlease, enter a frequency: ");
	check_double_data(&frequency);

	printf("Please enter a phase: ");
	check_double_data(&phase);

	buffer_size = sampling_rate * seconds;
	frequency_radians_per_sample = frequency * 2 * 3.14 / sampling_rate; // Frequency in radians

	if ((buffer = (uint16_t*)malloc(sizeof(uint16_t) * buffer_size)) == NULL) // If memory was not alloced
	{
		puts("\033[31mSory, but OS couldn\'t share a dynamic memory!");
		exit(EXIT_FAILURE);
	}

	printf("\033[32mPlease, enter a path to the csv file will be created: ");
	scanf("%s", path2);
	getchar();

	if ((csv = fopen(path2, "w")) == NULL)
	{
		printf("\033[31mSorry, it is undefined error.\n");
		exit(EXIT_FAILURE);
	}

	fputc(' ', csv);
	fputc(',', csv);
	fputc(' ', csv);
	fputc(',', csv);
	fprintf(csv, "%" PRIu16, seconds);
	fputc('\n', csv);

	printf("\033[32mPlease, choose the form of signal:\n");
	puts("1. Sinus");
	puts("2. Thiangle\n");

	while ((type = getchar()) != '1' && type != '2') // Make a choice which function will be in the signal base
	{
		printf("\033[31mSorry, you have entered incorrect data!\n");
		while (getchar() != '\n')
			continue;
	}

	while (getchar() != '\n')
		continue;

	if (type == '1')
		for (register uint32_t i = 0; i < buffer_size; i++)
		{
			phase += frequency_radians_per_sample;
			buffer[i] = (uint16_t)(amplitude * sin(phase)); // Counting data
			fprintf(csv, "%lf", phase);
			fputc(',', csv);
			fprintf(csv, "%" PRIu16, buffer[i]);
			fputc('\n', csv);
		}
	else
		for (register uint32_t i = 0; i < buffer_size; i++)
		{
			phase += frequency_radians_per_sample;
			buffer[i] = (uint16_t)(amplitude * (2 / 3.14) * asin(sin(2 * 3.14 * phase * i)));
			fprintf(csv, "%lf", phase);
			fputc(',', csv);
			fprintf(csv, "%" PRIu16, buffer[i]);
			fputc('\n', csv);
		}

	printf("\033[32mPlease, enter a path to the wav file will be created: ");
	scanf("%s", path1);
	getchar();

	write_wav(path1, buffer_size, buffer, sampling_rate);
	free(buffer); // free dynamic memory
	fclose(csv);
}

void check_double_data(double* data) // Checking double entered data
{
	while (!(scanf("%lf", data)) || getchar() != '\n')
	{
		puts("\033[31mSorry, you have entered incorrect data!");
		printf("\033[32mEnter again: ");

		while (getchar() != '\n')
			continue;
	}
}

void check_int_data(uint16_t* data) // Checking integer entered data
{
	while (!(scanf("%" SCNu16, data)) || getchar() != '\n')
	{
		puts("\033[31mSorry, you have entered incorrect data!");
		printf("\033[32mEnter again: ");

		while (getchar() != '\n')
			continue;
	}
}

void write_byte_per_byte(unsigned int word, int num_bytes, FILE* wav_file) // Write data byte per byte
{
	unsigned buffer;
	while (num_bytes > 0)
	{
		buffer = word & 0xff; // Separate one data byte
		fwrite(&buffer, 1, 1, wav_file); // Enter data byte by byte
		num_bytes--;
		word >>= 8;
	}
}


void write_wav(const char* filename, unsigned long num_samples, short int* data, int sampling_rate)
{
	FILE* wav_file;
	unsigned int channels_number;
	unsigned int bytes_per_sample;
	unsigned int byte_rate;

	channels_number = 1;
	bytes_per_sample = 2;


	byte_rate = sampling_rate * channels_number * bytes_per_sample;

	if ((wav_file = fopen(filename, "w")) == NULL)
	{
		puts("\033[31mIt happends undefined error!");
		exit(EXIT_FAILURE);
	}

	/* write RIFF header */
	fwrite("RIFF", 1, 4, wav_file); // Marks the file as a riff file
	write_byte_per_byte(36 + bytes_per_sample * num_samples * channels_number, 4, wav_file); // File size
	fwrite("WAVE", 1, 4, wav_file); // File Type Header

	fwrite("fmt ", 1, 4, wav_file); // Format chunk marker
	write_byte_per_byte(16, 4, wav_file); // Length of format data as listed above
	write_byte_per_byte(1, 2, wav_file); // Type of format (1 is PCM) – 2 byte integer
	write_byte_per_byte(channels_number, 2, wav_file); // Number of Channels – 2 byte integer
	write_byte_per_byte(sampling_rate, 4, wav_file); // Sample Rate
	write_byte_per_byte(byte_rate, 4, wav_file); // (Sample Rate * BitsPerSample * Channels) / 8
	write_byte_per_byte(channels_number * bytes_per_sample, 2, wav_file);  /* block align */
	write_byte_per_byte(8 * bytes_per_sample, 2, wav_file); // Bits per sample

	// WRITE A DATA
	fwrite("data", 1, 4, wav_file); // data chunk header, marks the beginning of the data section
	write_byte_per_byte(bytes_per_sample * num_samples * channels_number, 4, wav_file); // Size of the data section
	for (uint64_t i = 0; i < num_samples; i++)
	{
		write_byte_per_byte((unsigned int)(data[i]), bytes_per_sample, wav_file); // Data entering
	}

	fclose(wav_file); // Close file
}




