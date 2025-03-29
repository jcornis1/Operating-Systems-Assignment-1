# Operating-Systems-Assignment-1

This is my submission for Assignment #1 for my Operating Systems class at Kent State University during Spring 2025. The assignment is to engage with the Producer-Consumer problem. The producer is to generate items onto a table that uses shared memory, and the consumer, if functioning correctly, should consume/pick up those items. The table can hold two items concurrently and no more. The producer waits when the table is complete, and the consumer waits when there is no items for it consume. Semaphores are used to synchronize the producer and the consumer, with mutual exclusion being implemented as well. Threads are used for both the producer and the consumer.

# How to Compile

In order to compile, please clone this repository and run the following instructions:
```
$ g++ consumer.cpp -pthread -lrt -o consumer
$ g++ producer.cpp -pthread -lrt -o producer
./producer & ./consumer &
```
# Example Output

10 sets will run when you run this particular program. Output will be different every time due to the random number generation and somewhat unpredictable (but legal) behaviour that can result in the producer producing once or twice before the consumer can catch up and consume what has been left for it. Here is a potential output, with thread numbers included:
```
[1] 8207
[2] 8208
Producer thread 124404262504128 produced: 71
Producer thread 124404262504128 produced: 16
Consumer thread 131553613055680 consumed: 71
Consumer thread 131553602569920 consumed: 16
Producer thread 124404272989888 produced: 3
Producer thread 124404262504128 produced: 65
Consumer thread 131553613055680 consumed: 3
Consumer thread 131553613055680 consumed: 65
Producer thread 124404272989888 produced: 45
Producer thread 124404272989888 produced: 17
Consumer thread 131553613055680 consumed: 45
Producer thread 124404262504128 produced: 23
Consumer thread 131553613055680 consumed: 17
Producer thread 124404272989888 produced: 50
Consumer thread 131553602569920 consumed: 23
Consumer thread 131553602569920 consumed: 50
Producer thread 124404262504128 produced: 3
Producer thread 124404272989888 produced: 31
Consumer thread 131553602569920 consumed: 3
Consumer thread 131553602569920 consumed: 31
```
