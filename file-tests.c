// CS201 Fall 2021 Assignment #5
// jdh 11-25-21

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileops.jdiscipi.h"

//----------------------------------------------------------
// call this one with a file that doesn't exist
//----------------------------------------------------------

int fileTestOne(FILE *fp) {
  PartRecord partRecord[4], rec;
  int position, rc, i;

  rc = initializeFile(fp);
  if (rc != 0) {
    printf("error initializing file\n");
    return 1;
  }

  partRecord[0].partNumber = 1234;
  strcpy(partRecord[0].partName, "widget");
  partRecord[0].quantity = 12;

  partRecord[1].partNumber = 45;
  strcpy(partRecord[1].partName, "pillow");
  partRecord[1].quantity = 2;

  partRecord[2].partNumber = 123;
  strcpy(partRecord[2].partName, "notebook");
  partRecord[2].quantity = 4;

  for (i=0; i<3; ++i) {
    position = writeRecord(fp, &partRecord[i]);
    if (position < 0) {
      printf("writeRecord() failed\n");
      return 1;
    }

    printf("wrote a record for (%s, %d, %d) at position %d\n",
           partRecord[i].partName, partRecord[i].partNumber, partRecord[i].quantity, position);
  }

  for (i=2; i<=3; ++i) {
    position = i;
    rc = readRecord(fp, position, &rec);
    if (rc != 0) {
      printf("readRecord() at position %d failed\n", position);
      return 1;
    }

    // this should be partRecord[i-1]

    if (rec.partNumber != partRecord[i-1].partNumber ||
        rec.quantity != partRecord[i-1].quantity ||
        strcmp(rec.partName, partRecord[i-1].partName)) {
      printf("ERROR: expected (%s, %d, %d) but got a mismatch\n",
             partRecord[i-1].partName, partRecord[i-1].partNumber, partRecord[i-1].quantity);
      return 1;
    }
    printf("pass: read (%s, %d, %d) from position %d\n",
           rec.partName, rec.partNumber, rec.quantity, position);
  }

  // now try to read the record at position #4: this should fail
  position = 4;
  rc = readRecord(fp, position, &rec);
  if (rc == 0) {
    printf("ERROR: readRecord() at position %d succeeded, but it should fail\n", position);
    return 1;
  } else {
    printf("pass: readRecord at position %d failed, as expected\n", position);
  }

  return 0;
}


//----------------------------------------------------------
// call this one with testFile.dat
//----------------------------------------------------------

int fileTestTwo(FILE *fp) {
  PartRecord rec;
  int position, rc, i;
  char name[PART_NAME_LENGTH];

  for (i=0; i<8; ++i) {
    position = i+1;
    rc = readRecord(fp, position, &rec);
    if (rc != 0) {
      printf("readRecord() at position %d failed\n", position);
      return 1;
    }

    sprintf(name, "part_%d", i+1);
    if (rec.partNumber != i+11 ||
        rec.quantity != i+1 ||
        strcmp(rec.partName, name)) {
      printf("ERROR: expected (%s, %d, %d) but got a mismatch\n",
             name, i+11, i+1);
      return 1;
    }
    printf("pass: read (%s, %d, %d) from position %d\n",
           rec.partName, rec.partNumber, rec.quantity, position);
  }

  // now try to read the record at position #9: this should fail
  position = 9;
  rc = readRecord(fp, position, &rec);
  if (rc == 0) {
    printf("ERROR: readRecord() at position %d succeeded, but it should fail\n", position);
    return 1;
  } else {
    printf("pass: readRecord at position %d failed, as expected\n", position);
  }

  return 0;
}
