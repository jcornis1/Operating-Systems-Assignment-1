#ifndef BUFFER_HPP
#define BUFFER_HPP

//Jonah Godfrey-Cornish
//Operating Systems Assignment #1
//buffer.hpp

#include <semaphore.h>

namespace Shared {
    struct Buffer {
        int data[2]; //Table that holds two items
        int head; //Index for consumption (front of the queue)
        int tail; //Index for production (rear of the queue)
        sem_t full; //Semaphore for full slots
        sem_t empty; //Semaphore for empty slots
        sem_t mutex; //Semaphore for mutual exclusion
    };
}

#endif