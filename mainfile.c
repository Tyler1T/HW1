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

int main(int argc, char const *argv[]) {
  struct Data theData;
  readInputFile(&theData);
  return 0;
}

void readInputFile(struct Data data){
  char line[100];
  FILE *file;
  file = fopen("items.txt", "r");  /* open the file for reading */
  data.serial = malloc(100 * sizeof(int));
  data.product = malloc(100 * sizeof(char) * 50);
  data.cost = malloc(100 * sizeof(float));
  data.company = malloc(100 * sizeof(char) * 20);
  int spot = 0;
  while(fgets(line, 100, file) != NULL){
    *(data.serial + spot) = spot;
    printf(line);
    /* get a line, up to 100 chars from fr.  done if NULL */
    readLineData(data, &line, spot);
  }
  fclose(file);
}

void readLineData(struct Data data, char *line, int serial){
    printf("Made it to the readDataLine");
    sscanf(line, "%i, %lf", *(data.cost+serial));
    printf("%lf\n", *(data.cost+serial));
}
