#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Author: Tyler Tucker
  Email:  tyler.m.tucker@okstate.edu
  Date:   2/11/2021
  Program Description:
*/

void readInputFile();
void readLineData();

struct Data{
  int *serial;
  char *product;
  float *cost;
  char *company;
};

int main() {
  struct Data theData;
  theData.serial = calloc(100 ,  sizeof(int));
  theData.product = calloc(100 ,  sizeof(char) * 50);
  theData.cost = calloc(100 ,  sizeof(float));
  theData.company = calloc(100 ,  sizeof(char) * 20);
  readInputFile(&theData);
  return 0;
}

void readInputFile(struct Data *data){
  char line[100];
  FILE *file;
  file = fopen("items.txt", "r");  /* open the file for reading */

  int spot = 0;
  /* get a line, up to 100 chars from fr.  done if NULL */
  while(fgets(line, 100, file) != NULL){
    //scan in line and put each bit of data into the correct place in the struct
    sscanf(line, "%s\t%[^\n]\t%lf\t%[^\n]", *(data->serial+spot), *(data->product+spot), *(data->cost+spot), *(data->company+spot));
    spot++;
    printf("READ");
  }
  fclose(file);
  printf("%s\n", *(data->serial+12));
}
