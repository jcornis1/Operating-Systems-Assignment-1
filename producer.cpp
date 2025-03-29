//Jonah Godfrey-Cornish
//Operating Systems Assignment #1
//producer.cpp

#include "buffer.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ctime>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

namespace Shared {
    const char* SHM_NAME = "/my_shared_memory";

    //This is the producer thread function
    void* produce(void* arg) {
        Buffer* buffer = static_cast<Buffer*>(arg);
        for (int i = 0; i < 5; ++i) { //Each thread will produce 5 items
            int item = rand() % 100;

            sem_wait(&buffer->empty); //Waits for an empty slot
            sem_wait(&buffer->mutex); //Locks the buffer

            buffer->data[buffer->tail] = item;
            std::cout << "Producer thread " << pthread_self() << " produced: " << item << '\n';
            buffer->tail = (buffer->tail + 1) % 2; //This is for circular incrementing

            sem_post(&buffer->mutex); //Unlocks the buffer
            sem_post(&buffer->full); //Signals that an item is available
        }
        
        return nullptr;
    }
}

int main() {
    //Cleans up any pre-existing shared memory
    shm_unlink(Shared::SHM_NAME);

    //Creates the necessary shared memory
    int fd = shm_open(Shared::SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        std::cerr << "Producer: Failed to open the shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Sets the size of the shared memory
    if(ftruncate(fd, sizeof(Shared::Buffer)) == -1) {
        std::cerr << "Producer: Failed to truncate the shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Maps shared memory
    Shared::Buffer* buffer = static_cast<Shared::Buffer*>(mmap(nullptr, sizeof(Shared::Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (buffer == MAP_FAILED) {
        std::cerr << "Producer: Mapping failed" << '\n';
        exit(EXIT_FAILURE);
    }

    //Initializes the buffer and semaphores
    buffer->head = 0;
    buffer->tail = 0;
    if (sem_init(&buffer->full, 1, 0) == -1 || sem_init(&buffer->empty, 1, 2) == -1 ||sem_init(&buffer->mutex, 1, 1) == -1) {
        std::cerr << "Producer: Semaphore initialization failed" << '\n';
        exit(EXIT_FAILURE);
    }
        
    //Seeds the random number generator
    srand(static_cast<unsigned>(time(nullptr)));

    //Creates two producer threads
    pthread_t threads[2];
    for (int i = 0; i < 2; ++i) {
        if (pthread_create(&threads[i], nullptr, Shared::produce, buffer) != 0) {
            std::cerr << "Producer: Failed to create thread " << i << '\n';
            exit(EXIT_FAILURE);
        }
    }

    //Waits for threads to finish
    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], nullptr);
    }

    //Cleans up everything
    munmap(buffer, sizeof(Shared::Buffer));
    close(fd);

    return 0;
}