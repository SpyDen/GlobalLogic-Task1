#ifndef OVERLAY_H_
#define OVERLAY_H_

void write_wav(const char* filename, unsigned long num_samples, short int* data, int sampling_rate);
void check_int_data(uint16_t*);

#endif