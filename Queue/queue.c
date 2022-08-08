#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "queue.h"

/*
 * node_t - node object of linked list 
 *
 * val 		- (void*) void pointer to item enqueued in queue
 * nextNode - (node_t*) a pointer to the next node in the linked list 
 *
 * A node gets initialized with the node_create() function below 
 */
typedef struct node {
	void* val;
	struct node *nextNode;
} node_t;

/*
 * queue_t defintion 
 *
 * length - (int) length of queue 
 * head   - (node_t*) pointer to head of queue
 * tail   - (node_t*) pointer to tail of queue
 * 
 */
struct queue {
	int length; 
	node_t *head;
	node_t *tail;
};

queue_t queue_create(void)
{
	queue_t queue = malloc(sizeof(struct queue));
	if (queue) {
		queue->length = 0;
		queue->head = NULL;
		queue->tail = NULL;
		return queue;
	}
	return NULL;
}

/*
 * node_create - allocate space for a new node
 * @data: data to be enqueued
 * 
 * Create a new object of type node_t and returns pointer to it 
 *
 * Return: Pointer to node. NULL in case of failure when allocating
 * the new node
 */ 
node_t *node_create(void *data)
{
	node_t *new_node = malloc(sizeof(node_t));
	if (new_node) {
		new_node->val = data;
		new_node->nextNode = NULL;
		return new_node;
	}
	return NULL;
}

int queue_destroy(queue_t queue)
{
	if(queue && queue->length == 0){
		free(queue);
		return 0;
	}
	return -1;
}

int queue_enqueue(queue_t queue, void *data)
{
	if(queue && data){
		// if queue is empty
		if(queue->length == 0){
			node_t *node = node_create(data);
			if (node) {
				queue->head = node;
				queue->tail = node;
				queue->length = 1;
				return 0;
			}
		}
		else{
			node_t *node = node_create(data);
			if (node) {
				queue->tail->nextNode = node;
				queue->length++;
				queue->tail = node;
				return 0;
			}
		}
	}
	return -1;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue && queue->length!=0 && data){
		*data = queue->head->val; 
		// only one node in queue
		if (queue->length == 1) {
			free(queue->head);
			queue->head = NULL;
			queue->tail = NULL;
		} else {
			// pop the head and free node
			node_t *old_head = queue->head;
			queue->head = old_head->nextNode;
			free(old_head);
		}

		queue->length--;
		return 0;
	}
	return -1;
}

int queue_delete(queue_t queue, void *data)
{
	if(queue && data){
		node_t *curr_node = queue->head;
		node_t *prev_node = NULL;
		// iterate through queue until reach end 
		while (curr_node) {
			// found data in queue
			if (curr_node->val == data) {
				// 1 item in the queue
				if (queue->length == 1) {
					free(queue->head);
					queue->head = NULL;
					queue->tail = NULL;
				} else {
					// current node is not the head 
					if (prev_node) { 
						// remove current node from linked list 
						prev_node->nextNode = curr_node->nextNode; 
						if (curr_node == queue->tail) {
							queue->tail = prev_node;
						}
					} else {
						queue->head = curr_node->nextNode;
					}
					free(curr_node);	
				}
				queue->length--;
				return 0;
			}
			// increment current and previous node pointers 
			prev_node = curr_node;
			curr_node = curr_node->nextNode;
		}
	}
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{	
	if (queue && func) {
		node_t *curr_node = queue->head;
		// iterate through nodes in queue
		while(curr_node) {
			// run function on node's value and return the value
			int status = func(queue, curr_node->val, arg);
			// func returns one, stop iteration
			if (status == 1) {
				if(*data == NULL){
					*data = curr_node->val;
				}
				return 0;
			}
			curr_node = curr_node->nextNode;
		}
	}
	return -1;
}

int queue_length(queue_t queue)
{
	if(queue){
		return (queue->length);
	}
	return -1;
}