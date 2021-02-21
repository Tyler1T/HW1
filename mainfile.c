#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define QUEUE_NAME   "/addition"
#define PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

#define MAX 9000000000 // 9 billion times

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

  struct Data *dataPTR = &theData;
  readInputFile(dataPTR);

  printf("How many customers are there?");
  for()

  /* size (in bytes) of shared memory object */
  const int SIZE = 10000;

  /* name of the shared memory object */
  const char *name = "Shared Data Pool";

  int pid = fork();	// make two processes

  /* shared memory file descriptor */
  int shm_fd;

  /* pointer to shared memory object */

  if (pid < 0){	// fork failed
    fprintf(stderr, "fork failed..\n");
    exit(1);
  }else if(pid == 0){ // Child process

    /* Create or open a shared memory object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    /* Set the shared memory object size */
    ftruncate(shm_fd, SIZE);

    /* Map the shared memory object into the current address space */
    dataPTR = mmap(0, SIZE,  PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //DO STUFF HERE

  }else{	// Parent process

    /* open the shared memory object */
    shm_fd = shm_open(name, O_RDONLY, 0666);

    wait(NULL);

    /* memory map the shared memory object */
    dataPTR = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    //DO STUFF HERE

    /* Unmap the shared memory */
    munmap(dataPTR, SIZE);

    /* Close the shared memory object */
    close(shm_fd);

    /* Delete the shared memory object */
    shm_unlink(name);
  }

  //int spot = 21;
  //test(&theData);
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
    //test(data, i);
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
