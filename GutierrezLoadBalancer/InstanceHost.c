/**
* delegates a new thread to process the square of the input and
 * set it for the user
*
* Completion time: 1
*
* @author Gutierrez
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "InstanceHost.h"

typedef struct host {
    struct job_node* data;
    struct job_node* next;
}host;

void* squareNumberRunner(void* data){
    struct job_node *node = (struct job_node *) data;

    while (node != NULL){
        *(node->data_result) = node->data * node->data;
        struct job_node* temp = node;
        node = node->next;
        free(temp);
    }

    pthread_exit(NULL);
}

struct host* host_create() {
    printf("Creating Host\n");
    host* h = (host*)malloc(sizeof(host));
    return h;
}

void host_destroy(host** h) {
    free(*h);
}

void host_request_instance(host* h, struct job_node* batch) {

    pthread_t p;
    pthread_create(&p, NULL, squareNumberRunner, batch);
    pthread_detach(p);
}