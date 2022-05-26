/******************************************************************************************
 * Assignment 4
 * Zhiyu Li		    #821009794			cssc1433
 * 04/21/22
 ******************************************************************************************/
#include <semaphore.h>

#ifndef A4_01_SHAREDATA_H
#define A4_01_SHAREDATA_H
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <streambuf>
#include <vector>
#include <utility>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <cstdlib>
#include <unistd.h>

#define INITNUM 0
#define CONVERMICROTOMILLI 1000
#define SIZEOFQUEUE 2
using namespace std;

struct broker{

    //semaphore for detect available slots
    sem_t availableSlots;
    //mutex for limit the thread
    sem_t mutex;
    //determine how many product haven't consumed
    sem_t unconsumed;
    //the most human driver request in a row
    sem_t humLimit;

    sem_t barrier;

    //false is the human, true is robot
    bool flag;
    bool conFlag;

    //2 type, 0 is human, 1 is robot
    int inRequestQueue[SIZEOFQUEUE];
    int produced[SIZEOFQUEUE];

    //0 cost-saving, 1 fasting-matching
    int consumed[SIZEOFQUEUE];

    int **totalConsumed;

    //default is 120
    int totalNumReq;

    //trace the number of produced and consumed
    int currProducedNum;
    int currConsumeddNum;

    //time of cost-saving
    int timeOfCost;
    //time of fast-matching
    int timeOfFast;
    //time of human driver
    int timeOfHu;
    //time of robot driver
    int timeOfRo;

    //queue for insert, and consumer can determine what
    //kind of driver is
    queue<int> buffer;
};



#endif //A4_01_SHAREDATA_H


