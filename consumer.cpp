/******************************************************************************************
 * Assignment 4
 * Zhiyu Li		    #821009794			cssc1433
 * 04/21/22
 ******************************************************************************************/
#include "consumer.h"

/*************************************************************************
 * consume the product
 * @param ptr pointer point to the broker struct
 *************************************************************************/
void* consumer(void* ptr){

    //Access the broker
    broker *tmp = (broker *) ptr;
    //Make the first thread is cost-saving
    bool isFast = tmp->conFlag;
    //second thread will become fast-matching
    tmp->conFlag = true;

    //while the current state not reach out the total consumed requests yet
    while (tmp->currConsumeddNum <=  tmp->totalNumReq){
        //increase the current state
        tmp->currConsumeddNum++;
        //down the unconsume for running the consumer
        sem_wait(&tmp->unconsumed);

        //down the mutex
        sem_wait(&tmp->mutex);

            //it is the cost-saving
            if(!isFast){
                //if the first of queue is human driver
                if(tmp->buffer.front() == HumanDriver){
                    //signal the human driver request limit
                    sem_post(&tmp->humLimit);
                    //update the current state  of human driver
                    tmp->inRequestQueue[HumanDriver]--;
                    tmp->consumed[CostAlgoDispatch]++;
                    tmp->totalConsumed[CostAlgoDispatch][HumanDriver]++;
                    //print out the current process
                    io_remove_type(CostAlgoDispatch,HumanDriver,tmp->inRequestQueue,tmp->consumed);
                }else if(tmp->buffer.front() == RoboDriver){
                    //update the current state  of robot driver
                    tmp->inRequestQueue[RoboDriver]--;
                    tmp->consumed[CostAlgoDispatch]++;
                    tmp->totalConsumed[CostAlgoDispatch][RoboDriver]++;
                    //print out the current process
                    io_remove_type(CostAlgoDispatch,RoboDriver,tmp->inRequestQueue,tmp->consumed);
                }

            }else{ //if the thread is fast-matching

                //if the first of queue is human driver
                if(tmp->buffer.front() == HumanDriver){
                    //signal the human driver request limit
                    sem_post(&tmp->humLimit);
                    //update the current state  of human driver
                    tmp->inRequestQueue[HumanDriver]--;
                    tmp->consumed[FastAlgoDispatch]++;
                    tmp->totalConsumed[FastAlgoDispatch][HumanDriver]++;

                    //print out the current process
                    io_remove_type(FastAlgoDispatch,HumanDriver,tmp->inRequestQueue,tmp->consumed);
                }else if(tmp->buffer.front() == RoboDriver){

                    //update the current state  of robot driver
                    tmp->inRequestQueue[RoboDriver]--;
                    tmp->consumed[FastAlgoDispatch]++;
                    tmp->totalConsumed[FastAlgoDispatch][RoboDriver]++;

                    //print out the current process
                    io_remove_type(FastAlgoDispatch,RoboDriver,tmp->inRequestQueue,tmp->consumed);
                }


            }



        //pop out the first of the queue
        tmp->buffer.pop();
        //signal the mutex
        sem_post(&tmp->mutex);
        //signal the available slots
        sem_post(&tmp->availableSlots);

        //determine whether it is cost-saving or fast-matching
        //and sleep the thread for a specific time
        if(!isFast){
            usleep(tmp->timeOfCost * CONVERMICROTOMILLI);
        }else{
            usleep(tmp->timeOfFast * CONVERMICROTOMILLI);
        }


    }
    //signal up the barrier every time the thread completed
    sem_post(&tmp->barrier);
    return NULL;
}