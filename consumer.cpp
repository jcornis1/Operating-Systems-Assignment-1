//Jonah Godfrey-Cornish
//Operating Systems Assignment #1
//consumer.cpp

#include "buffer.hpp"
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

namespace Shared {
    const char* SHM_NAME = "/my_shared_memory";

    //This is the consumer thread function
    void* consume(void* arg) {
        Buffer* buffer = static_cast<Buffer*>(arg);
        for (int i = 0; i < 5; ++i) {
            sem_wait(&buffer->full); //Waits for an item
            sem_wait(&buffer->mutex); //Locks the buffer

            int item = buffer->data[buffer->head];
            buffer->data[buffer->head] = 0; //Clears the consumed slot
            buffer->head = (buffer->head + 1) % 2; //Allows for circular incrementation
            std::cout << "Consumer thread " << pthread_self() << " consumed: " << item << '\n';

            sem_post(&buffer->mutex); //Unlocks the buffer
            sem_post(&buffer->empty); //Signals an empty slot
        }

        return nullptr;
    }
}

int main () {
    //Briefly delays in order to guarantee that the producer initializes shared memory
    sleep(1);

    //Opens the shared memory
    int fd = shm_open(Shared::SHM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        std::cerr << "Consumer: Failed to open the shared memory" << '\n';
        exit(EXIT_FAILURE);
    }

    //Maps the shared memory
    Shared::Buffer* buffer = static_cast<Shared::Buffer*>(mmap(nullptr, sizeof(Shared::Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if (buffer == MAP_FAILED) {
        std::cerr << "Consumer: Mapping failed" << '\n';
        exit(EXIT_FAILURE);
    }

    //Creates two consumer threads
    pthread_t threads[2];
    for (int i = 0; i < 2; ++i) {
        if (pthread_create(&threads[i], nullptr, Shared::consume, buffer) != 0) {
            std::cerr << "Consumer: Failed to create thread " << i << '\n';
            exit(EXIT_FAILURE);
        }
    }

    //Waits for the threads to finish
    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], nullptr);
    }

    //Cleans up everything
    munmap(buffer, sizeof(Shared::Buffer));
    close(fd);
    shm_unlink(Shared::SHM_NAME);

    return 0;
}