#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct char_print_parms
{
    char character;
    int count;
};

void* char_print(void* param)
{
    struct char_print_parms* p = (struct char_print_parms*) param;
    
    for(int i = 0; i < p->count; ++i)
        fputc(p->character, stderr);

    return NULL;
}

int main(void)
{
    pthread_t thread1_id; 
    pthread_t thread2_id;
    struct char_print_parms thread1_args = {'x', 3000}; 
    struct char_print_parms thread2_args = {'o', 2000};

    pthread_create(&thread1_id, NULL, &char_print, &thread1_args);
    pthread_create(&thread2_id, NULL, &char_print, &thread2_args);

    pthread_join(thread1_id, NULL);
    pthread_join(thread2_id, NULL);

    return EXIT_SUCCESS;
}