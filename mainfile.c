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
#include <mqueue.h>
#include <sys/msg.h>


#define QUEUE_NAME   "/communication"
#define PERMISSIONS 0660
#define MAX_MESSAGES 100
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

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
void customer();
void helper();

int main() {
  struct Data theData;
  struct Data *dataPTR = &theData;
  readInputFile(dataPTR);

  /* size (in bytes) of shared memory object */
  const int SIZE = 10000;

  /* name of the shared memory object */
  const char *name = "Shared Data Pool";

  mqd_t qd;   // queue descriptors
  mqd_t *que = &qd; // queue pointer to pass to helper and customer

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MESSAGES;	// The maximum number of messages that can be stored on the queue.
  attr.mq_msgsize = MAX_MSG_SIZE;	// The maximum size of each message on the given message queue.
  attr.mq_curmsgs = 0;	// This field represents the number of messages currently on the given queue.

  //Getting customers and order of process for customers
  printf("How many customers are there?\n");
  int customers = 0;
  scanf("%d", &customers);
  printf("What order should the customers be helped?\n");
  printf("Please enter a list seperated by spaces to show customer order\n");
  printf("Ex: 2 6 4 1 3 5\n");
  uint *order;
  order = malloc(customers * sizeof(uint));
  for(int i  = 0; i < customers; i++){
    scanf("%d", &order);
  }

  /* shared memory file descriptor */
  int shm_fd;

  for(int i = customers; i >= 0; i--){
      if(fork() == 0){

        /* Create or open a shared memory object */
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

        /* Set the shared memory object size */
        ftruncate(shm_fd, SIZE);

        /* Map the shared memory object into the current address space */
        dataPTR = mmap(0, SIZE,  PROT_WRITE, MAP_SHARED, shm_fd, 0);

        if(i == 0){ // only need to create 1 Helper process
          helper(dataPTR, &order, que, &attr);
          exit(0);
        }else{ // all other processes are customers
          customer(dataPTR, i, que, &attr);
          exit(0);
        }
      }else if(getpid() < 0){ // if fork fails
        fprintf(stderr, "fork failed..\n");
        exit(1);
      }else{ // waiting for child processes to finish
        wait(NULL);
      }
  }


  printf("Thank you!\n");
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

void customer(struct Data *data, int num, mqd_t *qd, struct mq_attr *attr){
  // getting number of gifts this customer will have
  int giftCount;
  time_t t;
  srand((unsigned) time(&t));
  printf("How many gifts does customer %d want?\n", getpid());
  scanf("%d", &giftCount);

  // setting gift array length and putting serial numbers in said array
  char buffer [MSG_BUFFER_SIZE];
  sprintf(buffer, "%d %d ", num, giftCount);
  for(int i = 2; i < giftCount + 2; i++){
    sprintf(buffer + strlen(buffer), "%d ", rand() % 100 + 1);
  }

  // opening que to write
  if (*qd = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT | O_NONBLOCK , PERMISSIONS, *attr) == -1) {
    perror ("Customer could not open the que");
    exit (1);
  }


  // putting customer information in que and sending
  if (mq_send(*qd, buffer, strlen(buffer) + 1, 0) == -1) {
    perror ("Customer could not send a message in the que");
    exit(1);
  }
}

void helper(struct Data *data, uint *order, mqd_t *qd, struct mq_attr *attr){
  // create and open que to read and write
  if ((*qd = mq_open(QUEUE_NAME, O_RDONLY, PERMISSIONS, *attr)) == -1) {
    perror ("Helper: mq_open");
    exit (1);
  }

  char buffer [MSG_BUFFER_SIZE];
  if (mq_receive(*qd, buffer, MSG_BUFFER_SIZE, NULL) == -1) {
    perror ("Helper: mq_receive");
    exit (1);
  }

  for(int i = 0; i < sizeof(buffer); i++){
    printf("%d", atoi(&buffer[i]));
  }

}
