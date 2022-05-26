/******************************************************************************************
 * Assignment 4
 * Zhiyu Li		    #821009794			cssc1433
 * 04/21/22
 ******************************************************************************************/
#include "consumer.h"
#include "ridesharing.h"
#include "io.h"
#include "producer.h"
#include "shareData.h"
#define DEFAULTBUFFERSIZE 120
#define SLOTS 12
#define HUMANNMOST 4
#define MUTEXNUM 1
#define INITOFPRODUCEANDCONSUME 1
using namespace std;
int main(int argc, char **argv) {

    //to get the optional argument
    int ch;

    //allocate memory for broker
    broker *ptr = new broker();

    //initialize mutex
    sem_init(&ptr->mutex, 0, MUTEXNUM);
    //initialize the available slots
    sem_init(&ptr->availableSlots, 0, SLOTS);
    //initialize the most human driver requests in a row
    sem_init(&ptr->humLimit, 0, HUMANNMOST);
    //initialize the product need to be consumed
    sem_init(&ptr->unconsumed, 0, INITNUM);

    sem_init(&ptr->barrier, 0, INITNUM);

    //allocate memory for double pointer
    ptr->totalConsumed = (int **)calloc(ConsumerTypeN, sizeof(int));;

    //initialize the flag to false, in order to determine it later
    ptr->flag = false;
    //initialize the flag to false for consumer function, in order to determine it later
    ptr->conFlag = false;
    //a for-loop to initialize the arrays
    for(int i = 0; i < RequestTypeN; i++){
        //waiting queue initialize
        ptr->inRequestQueue[i] = INITNUM;
        //consumed requests
        ptr->consumed[i] = INITNUM;
        //produced requests
        ptr->produced[i] = INITNUM;
        //every index is point to an array that is indexed by request type
        ptr->totalConsumed[i] = (int*)calloc(ConsumerTypeN, sizeof(int));
        for(int j = 0; j < RequestTypeN; j++){
            //initialize the array by request type
            ptr->totalConsumed[i][j] = INITNUM;
        }
    }

    //total number of requests
    ptr->totalNumReq = DEFAULTBUFFERSIZE;

    //time to consume
    ptr->timeOfCost = INITNUM;
    ptr->timeOfFast = INITNUM;

    //time to produce
    ptr->timeOfHu = INITNUM;
    ptr->timeOfRo = INITNUM;

    //current processing state
    ptr->currProducedNum = INITOFPRODUCEANDCONSUME;
    ptr->currConsumeddNum = INITOFPRODUCEANDCONSUME;


    //get the optional argument
    while((ch = getopt(argc, argv, "n:c:f:h:a:")) != -1 ){
        switch(ch){
            case 'n' :
                ptr->totalNumReq =  (int) atoi(optarg);
                //printf("the total num is %d\n", ptr->totalNumReq);
                break;
            case 'c':
                ptr->timeOfCost = (int) atoi(optarg);
                //printf("the time fo costsaving is %d\n", ptr->timeOfCost);
                break;
            case 'f':
                ptr->timeOfFast = (int) atoi(optarg);
                //printf("the time fo fasting is %d\n", ptr->timeOfFast);
                break;
            case 'h':
                ptr->timeOfHu = (int) atoi(optarg);
                //printf("the time fo human is %d\n", ptr->timeOfHu);
                break;
            case 'a':
                ptr->timeOfRo = (int) atoi(optarg);
                //printf("the time fo Robot is %d\n", ptr->timeOfRo);
                break;
            default:
                break;
        }
    }

    //threads
    pthread_t human, robot, costS, fastM;

    //sem_wait(&ptr->barrier);

    //create the threads of human driver, robot driver, cost-saving consumer
    //and fast-matching consumer



    pthread_create(&human,NULL, producer, (void*)ptr);
    //sem_post(&ptr->barrier);


    pthread_create(&robot,NULL, producer, (void*)ptr);
    //sem_post(&ptr->barrier);


    pthread_create(&costS, NULL, consumer, (void*)ptr);
    //sem_post(&ptr->barrier);


    pthread_create(&fastM, NULL, consumer, (void*)ptr);
    //sem_post(&ptr->barrier);


    //down the main thread, which let the main thread wait for other to completed first
    sem_wait(&ptr->barrier);
    sem_wait(&ptr->barrier);
    sem_wait(&ptr->barrier);
    sem_wait(&ptr->barrier);

    //join the thread in order to let them complete
//    pthread_join(human, NULL);
//    pthread_join(robot, NULL);
//    pthread_join(costS, NULL);
//    pthread_join(fastM, NULL);


    //report the process at the end
    io_production_report(ptr->produced, ptr->totalConsumed);

    //destroy all the semaphore
    sem_destroy(&ptr->mutex);
    sem_destroy(&ptr->availableSlots);
    sem_destroy(&ptr->humLimit);
    sem_destroy(&ptr->unconsumed);
    sem_destroy(&ptr->barrier);


    //delete the pointer
    delete(ptr);
    delete(ptr->totalConsumed);

    return 0;

}
