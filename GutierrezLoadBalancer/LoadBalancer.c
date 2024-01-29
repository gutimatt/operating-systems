/**
* controls creating batches to send to instance host
*
* Completion time: 4
*
* @author Gutierrez
* @version Gutierrez
*/

////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "LoadBalancer.h"

typedef struct balancer {
    pthread_mutex_t list_lock;
    int batch_size;
    int count;
    struct host* host;
    struct job_node* head;
}balancer;


balancer* balancer_create(int batch_size){
    balancer* lb = (balancer*)malloc(sizeof(balancer));
    pthread_mutex_init(&lb->list_lock, NULL);
    lb->head = NULL;
    lb->batch_size = batch_size;
    lb->count = 0;
    lb->host = host_create();
    return lb;
}

void balancer_destroy(balancer** lb) {
    printf("Destroying balancer\n");

    if ((*lb)->head != NULL) {
        host_request_instance((*lb)->host, (*lb)->head);
    }

    host_destroy(&(*lb)->host);
    pthread_mutex_destroy(&(*lb)->list_lock);
    free(*lb);
}

void balancer_add_job(balancer* lb, int user_id, int data, int* data_return) {
    struct job_node *node = (struct job_node *) malloc(sizeof(struct job_node));

    node->user_id = user_id;
    node->data = data;
    node->data_result = data_return;

    pthread_mutex_lock(&lb->list_lock);
        node->next = lb->head;
        lb->head = node;
        if (lb->count < lb->batch_size-1){
            lb->count = lb->count + 1;
        } else {
            host_request_instance(lb->host, lb->head);
            lb->head = NULL;
            lb->count = 0;
        }
    pthread_mutex_unlock(&lb->list_lock);
}