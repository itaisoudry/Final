#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */

/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef struct sp_bpq_element_t {
	int index;
	double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY, SP_BPQUEUE_FULL, SP_BPQUEUE_EMPTY, SP_BPQUEUE_INVALID_ARGUMENT, SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * create new queue
 * @param maxSize size of queue
 * @return NULL if maxSize < 0 or one of the memory allocation failed, otherwise a new queue.
 */
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * Copy queue
 * @param source  -source to copy
 * @return a new queue or NULL if error occured
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 * @param source - queue to destroy ( free memory )
 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 * Empty queue.
 * @parama source - queue to empty
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * @param source - queue
 * @return queue size ( num of elements in queue )
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * @param source - queue
 * @return the maximum size of the queue, -1 otherwise
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 * Insert a new element to the queue.
 * The queue is sorted by value, so the new element will be inserted by its value in the correct place.
 * @return  SP_BPQUEUE_INVALID_ARGUMENT - if source==NULL or index<0
 *  		SP_BPQUEUE_OUT_OF_MEMORY - if memory allocation failed
 *  		SP_BPQUEUE_FULL - if queue is full
 *  		SP_BPQUEUE_SUCCESS - if enqueue succeeded.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * Removes the first element from the queue( using only the start pointer )
 * @return SP_BPQUEUE_INVALID_ARGUMENT - if source==NULL
 * 		   SP_BPQUEUE_EMPTY - if queue is empty.
 * 		   SP_BPQUEUE_SUCCESS - if dequeue succeeded.
 *
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**Getting a copy of the first element in the queue.
 * The first element will be returned into the argument res.
 *  @return SP_BPQUEUE_SUCCESS if the first element found \ exists
 *  		SP_BPQUEUE_INVALID_ARGUMENT if res is NULL
 *  		SP_BPQUEUE_EMPTY if queue is empty
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 * Getting a copy of the last element in the queue.
 * The last element will be returned into the argument res.
 * @return 	 SP_BPQUEUE_SUCCESS if the last element found \ exists
 * 			 SP_BPQUEUE_INVALID_ARGUMENT if res is null
 * 			 SP_BPQUEUE_EMPTY if queue is empty
 *
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/**
 * Returns the min value in the queue
 * @return min value found, if not found or memory allocation failed, -1.
 */
double spBPQueueMinValue(SPBPQueue* source);

/**
 * Returns the max value in the queue.
 * @return max value found, if not found or memory allocation failed, -1.
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 * Checks if queue is empty
 * @return true if queue is empty, false otherwise
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 * Checks if queue is full.
 * @return true if queue is full, false otherwise.
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif
