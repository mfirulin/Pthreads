#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define FILE_NAME_LEN 20
#define NUM_OF_THREADS 5

static pthread_key_t thread_log_key;

void write_to_thread_log(const char* message)
{
    FILE* thread_log = (FILE*)pthread_getspecific(thread_log_key);
    fprintf(thread_log, "%s\n", message);
}

void close_thread_log(void* thread_log)
{
    fclose((FILE*)thread_log);
}

void* thread_function(void* args)
{
    char thread_log_filename[FILE_NAME_LEN];
    FILE* thread_log;

    pthread_t thread = pthread_self();
    snprintf(thread_log_filename, FILE_NAME_LEN, "thread%d.log", (int)thread.x);
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);

    write_to_thread_log("Thread starting");
    
    /* Other thread work here... */

    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_OF_THREADS];
    
    pthread_key_create(&thread_log_key, close_thread_log);

    for (int i = 0; i < NUM_OF_THREADS; ++i) {
        pthread_create(&(threads[i]), NULL, &thread_function, NULL);
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}