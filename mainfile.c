#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>

/*
  Author: Tyler Tucker
  Email:  tyler.m.tucker@okstate.edu
  Date:   2/11/2021
  Program Description:
*/

struct Data{
  int *serial;
  char **product;
  double *cost;
  char **company;
};

void readInputFile();
void test();

int main() {
  struct Data theData;
  readInputFile(&theData);
  int spot = 21;
  test(&theData);
  //printf("%d %s $%.2lf %s\n", theData.serial[spot], theData.product[spot][5], theData.cost[spot], theData.company[spot]);
  return 0;
}

void readInputFile(struct Data *data){
  char line[100]; //string buffer
  char *tempC; // company buffer
  char *tempP; // product buffer
  tempC = malloc(20 * sizeof(char));
  tempP = malloc(50 * sizeof(char));
  data->serial = malloc(100 * sizeof(int));
  data->product = malloc(100 * sizeof(tempP));
  data->cost = malloc(100 * sizeof(double));
  data->company = malloc(100 * sizeof(tempC));

  FILE *file; // file object creation
  file = fopen("items.txt", "r");  //open the file for reading

  /*
    Get a line, up to 100 chars from items.txt and parse the information
    to put into the struct of all the data
  */
  for(int i = 0; i < 100; i++){
    fgets(line, 100, file);
    sscanf(line, "%d%*c\t%[^\t] $%lf at %[^\n]", &data->serial[i],
        tempP, &data->cost[i], tempC);
    data->product[i] = tempP;
    data->company[i] = tempC;
    test(data, i);
    //printf("%d %s $%.2lf %s\n", data->serial[i], data->product[i], data->cost[i], data->company[i]);
  }

  // Deallocate memory from temporary arrays and file object
  free(tempP);
  free(tempC);
  fclose(file);
}

void test(struct Data *data, int num){
  printf("%d %s $%.2lf %s\n", data->serial[num], data->product[num], data->cost[num], data->company[num]);

}
