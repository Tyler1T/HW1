#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Author: Tyler Tucker
  Email:  tyler.m.tucker@okstate.edu
  Date:   2/11/2021
  Program Description:
*/

struct Data{
  int *serial;
  char *product;
  double *cost;
  char *company;
};

void readInputFile();

int main() {
  struct Data theData;
  theData.serial = malloc(100 * sizeof(int));
  theData.product = malloc(100 * sizeof(char) * 50);
  theData.cost = malloc(100 * sizeof(double));
  theData.company = malloc(100 * sizeof(char) * 20);
  readInputFile(&theData);
/*
  printf("%d\n", theData.serial);
  printf("%s\n", theData.product);
  printf("$%.2f\n", theData.cost);
  printf("%s\n", theData.company);*/
  return 0;
}

void readInputFile(struct Data *data){
  char line[100]; //string buffer
  FILE *file;
  file = fopen("items.txt", "r");  //open the file for reading
  int spot = 5;
  fgets(line, 100, file);
  fgets(line, 100, file);
  fgets(line, 100, file);
  fgets(line, 100, file);
  fgets(line, 100, file);


  char test[] = "$123.3";
  float num=0;
  sscanf(test, "$%lf", num);
  printf("$%.2lf\n", num);
  //scan in line and put each bit of data into the correct place in the struct
  sscanf(line, "%d%*c\t%[^$]$%lf at %[^\n]", data->serial+spot,
          data->product+spot, data->cost+spot, data->company+spot);
  printf("%d\n", data->serial+spot);
  printf("%s\n", data->product+spot);
  printf("$%.2f\n", data->cost+spot);
  printf("%s\n", data->company+spot);
  //get a line, up to 100 chars from items.txt done if NULL
  while(spot < 100){
    fgets(line, 100, file);
    //scan in line and put each bit of data into the correct place in the struct
    sscanf(line, "%d%*c\t%[^$]$%lf at %[^\n]", &data->serial+spot,
        data->product+spot, &data->cost+spot, data->company+spot);
    spot++;
  }
/*  printf("%d\n", data->serial);
  printf("%s\n", data->product);
  printf("$%.2f\n", data->cost);
  printf("%s\n", data->company);*/

  fclose(file);
}
