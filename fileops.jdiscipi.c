#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "fileops.jdiscipi.h"


int initializeFile(FILE* fp) {
	// Check that the pointer is not null
    assert(fp != NULL);

	// Go to the start of the file
    int seeked = fseek(fp, 0, SEEK_SET);
    if(seeked != 0) return 1;

	// Write 0 to the first 4 bytes
    int* nil = (int*) malloc(sizeof(int));
    *nil = 0;
    int num_obs_written = fwrite(nil, sizeof(nil), 1, fp);
    if(num_obs_written == 1) return 0;
    return 1;
}


int writeRecord(FILE *fp, PartRecord *partData) {
	// Go to the start of the file
	int seeked = fseek(fp, 0, SEEK_SET);
	if(seeked != 0) return -1;

	int* count = (int*) malloc(sizeof(int));
	*count = 0;

	// Read the first 4 bytes
	// This int represents the number of records
	size_t num_obs_read = fread(count, sizeof(int), 1, fp);
	if(num_obs_read != 1) return -1;

	// Calculate position to write to in file (byte index)
	int pos = (*count == 0)? 4 : 4 + (*count - 1) * sizeof(PartRecord);

	// Go to that position
	seeked = fseek(fp, pos, SEEK_SET);
	if(seeked != 0) return -1;
	
	// Write the contents of partData (PartRecord) to the file starting at the pos'th byte
	size_t num_obs_written = fwrite(partData, sizeof(PartRecord), 1, fp);

	// Need to also increment count
	if(num_obs_written == 1) {
		// Go to the start of the file
		seeked = fseek(fp, 0, SEEK_SET);
		if(seeked != 0) return -1;
		// Increment count and store to file
		*count += 1;
		num_obs_written = fwrite(count, sizeof(int), 1, fp);
		if(num_obs_written == 1) return 0;
		return -1;
	}
	return -1;
}

int readRecord(FILE *fp, int position, PartRecord *partData) {
	// Check that the file pointer is not null
	assert(fp != NULL);

	// Check that position will not cause offset issues
	assert(position > 0);

	// Calculate offset where we should read from file
	int offset = 4 + (position - 1) * sizeof(PartRecord);

	// Check how many records are in the binary
	int seeked = fseek(fp, 0, SEEK_SET);
	if(seeked != 0) return 1;
	int* count = (int*) malloc(sizeof(int));
	*count = 0;
	size_t num_obs_read = fread(count, sizeof(int), 1, fp);
	if(num_obs_read != 1) return 1;
	// Verify there are at least position records in the binary
	if(*count < position) return 1;

	// Read the record from the offset
	seeked = fseek(fp, offset, SEEK_SET);
	if(seeked != 0) return 1;
	num_obs_read = fread(partData, sizeof(PartRecord), 1, fp);
	if(num_obs_read == 1) return 0;
 
	return 1;
}



/*
Linux I/O Calls:

FILE* fopen(const char *path, const char *mode);
int fclose(FILE *fp);
int fseek(FILE *stream, long offset, int whence);
size_t fread(void *ptr, size_t size, size_t num, FILE *fp);
size_t fwrite(void *ptr, size_t size, size_t num, FILE *fp);
*/
int main() {
	const char* filename = "testFile.dat";
	int fileExists = 0;
	FILE* fp = fopen(filename, "r+"); // open for read and write, if the file already exists if (fp != NULL)
	fileExists = 1;
	if ( ! fileExists ) {
	fp = fopen(filename, "w+"); // open for read and write and create the file if (fp == NULL) {
		printf("cannot open file '%s'\n", filename);
		return 8; 
	}
	
	int return_code = initializeFile(fp);
	printf("Init File: %d\n", return_code);

	PartRecord pr = { .partNumber = 115, .partName = "Pillow", .quantity = 11 };
	return_code = writeRecord(fp, &pr);
	printf("Write record: %d\n", return_code);

	PartRecord* retrieved = (PartRecord*) malloc(sizeof(PartRecord));
	return_code = readRecord(fp, 1, retrieved);
	printf("Read record: %d\n", return_code);
	if(return_code == 0) {
		printf("Part number: %d, Part name: %s, Quantity: %d\n", retrieved->partNumber, retrieved->partName, retrieved->quantity);
	}

	return 0;
}
