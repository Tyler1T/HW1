#include <stdio.h>
#include <stdlib.h>

/*
  Author: Tyler Tucker
  Email:  tyler.m.tucker@okstate.edu
  Date:   2/11/2021
  Program Description:
*/

void readInputFile();
void readLineData();

struct Data{
  int length;
  string *product;
  int *cost;
  string *company;
};

void int main(int argc, char const *argv[]) {
  struct Data theData;
  readInputFile(theData);

  return 0;
}

void readInputFile(struct Data data){
  char line[100];
  FILE *file;
  file = fopen ("items.txt", "r");  /* open the file for reading */
  data->length = 0;
  while(fgets(line, 100, file) != NULL){
    data->length++;
    /* get a line, up to 80 chars from fr.  done if NULL */
    sscanf(line);
    readLineData(&line);
  }
  fclose(file);
}

void readLineData(struct Data data, char[] *line){

}
