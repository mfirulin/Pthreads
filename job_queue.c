#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_OF_THREADS 2

struct job
{
    struct job* next;
    /* other fields here ... */
};

static struct job* job_queue;
static pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t job_queue_count;

void initialize_job_queue()
{
    job_queue = NULL;
    sem_init(&job_queue_count, 0, 0);
}

void* process_job(void* param)
{
    while (1) {
        struct job* next_job;
        
        sem_wait(&job_queue_count);
        
        pthread_mutex_lock(&job_queue_mutex);
        next_job = job_queue;
        job_queue = job_queue->next;        
        pthread_mutex_unlock(&job_queue_mutex);
        
        if (next_job) { 
            // Work with next_job here ...        
            free(next_job);
        }
        
        // Logging        
        pthread_t thread = pthread_self();
        printf("thread.%d takes a job\n", (int)thread.x); 
    }
    return NULL;
}

void* enqueue_job(void* param)
{
    struct job* new_job;
    
    while (1) {
        new_job = malloc(sizeof(struct job));
        
        pthread_mutex_lock(&job_queue_mutex);
        new_job->next = job_queue;
        job_queue = new_job;
        sem_post(&job_queue_count);
        pthread_mutex_unlock(&job_queue_mutex);
        
        // Logging        
        pthread_t thread = pthread_self();
        printf("thread.%d creates a job\n", (int)thread.x);
    }
    return NULL;
}

int main(void)
{
    initialize_job_queue();
    
    pthread_t workers[NUM_OF_THREADS];
    pthread_t manager;

    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        pthread_create(&(workers[i]), NULL, &process_job, NULL);
        pthread_join(workers[i], NULL);
    }
        
    pthread_create(&manager, NULL, &enqueue_job, NULL);
    pthread_join(manager, NULL);

    return EXIT_SUCCESS;
}
