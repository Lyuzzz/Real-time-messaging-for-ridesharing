/******************************************************************************************
 * Assignment 4
 * Zhiyu Li		    #821009794			cssc1433
 * 04/21/22
 ******************************************************************************************/
#include "producer.h"

using namespace std;

/*************************************************************************
 * produce the product
 * @param ptr pointer point to the broker struct
 *************************************************************************/
void *producer(void *ptr) {

    //Access the broker
    broker *tmp = (broker *) ptr;
    //Make the first thread is Human driver
    bool isRobt = tmp->flag;
    //second thread will become robot driver
    tmp->flag = true;

    //while the current state not reach out the total requests yet
    while (tmp->currProducedNum <= tmp->totalNumReq) {
        //update the current state
        tmp->currProducedNum++;

        //if the current thread is human driver request
        if (!isRobt) {
            //sleep the thread for a specific time
            usleep(tmp->timeOfHu * CONVERMICROTOMILLI);
            //down the most human driver request
            sem_wait(&tmp->humLimit);
        } else {//id the current thread is robot driver request
            //sleep the thread for a specific time
            usleep(tmp->timeOfRo * CONVERMICROTOMILLI);
        }

        //down the available slots
        sem_wait(&tmp->availableSlots);

        //down the mutex
        sem_wait(&tmp->mutex);
        //printf("the currProducedNum value is %d\n", tmp->currProducedNum);

        //if the current thread is human driver request
        if (!isRobt) {
            //push the tpye of request into queue
            tmp->buffer.push(HumanDriver);
            //update the current state  of human driver
            tmp->inRequestQueue[HumanDriver]++;
            tmp->produced[HumanDriver]++;

            //print out the current process
            io_add_type(HumanDriver, tmp->inRequestQueue, tmp->produced);
        } else if (isRobt) { //if the current thread is robot driver request

            //push the tpye of request into queue
            tmp->buffer.push(RoboDriver);
            //update the current state  of robot driver
            tmp->inRequestQueue[RoboDriver]++;
            tmp->produced[RoboDriver]++;

            //print out the current process
            io_add_type(RoboDriver, tmp->inRequestQueue, tmp->produced);
        }


    //signal the mutex
    sem_post(&tmp->mutex);


    //signal the unconsumed
    sem_post(&tmp->unconsumed);



    }

    //signal up the barrier every time the thread completed
    sem_post(&tmp->barrier);
    return NULL;





}

