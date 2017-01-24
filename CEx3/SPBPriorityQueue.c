#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


//**SP ELEMENT**//

struct sp_bp_queue_t{
	int maxSize;
	BPQueueElement** list;
	int size;
};
/**
 * Allocates a new element in the memory, with index and value as specified.
 * @assert(value>=0)
 * @return -	NULL if memory allocation failure occured
  				Otherwise, returns pointer to the new elemnt
 */
BPQueueElement* spElementCreate(int index, double value){
	BPQueueElement* res = (BPQueueElement*) malloc(sizeof(*res));
	if(res == NULL) return NULL;
	res->index = index;
	res->value = value;
	return res;
}

int spElementGetIndex(BPQueueElement* elem){
	if(elem==NULL) return -1;
	return elem->index;
}


//**SP BP QUEUE**//

SPBPQueue* spBPQueueCreate(int maxSize){
	assert(maxSize > 0);
	SPBPQueue* res = (SPBPQueue*) malloc(sizeof(*res)); // sizeof(*res) == sizeof(SPBPQueue)
	if(res == NULL) return NULL;
	res->maxSize = maxSize;
	res->size = 0;
	res->list = (BPQueueElement**) malloc(sizeof(BPQueueElement*) * maxSize);
	if(res->list == NULL) {
		free(res);
		return NULL;
	}
	return res;
}

SPBPQueue* spBPQueueCopy(SPBPQueue* source){
	assert (source!=NULL);
	SPBPQueue* res = spBPQueueCreate(source->maxSize);
	if(res == NULL) return NULL;
	res->size = source->size;
	for(int i=0;i<(res->size);i++){
		(res->list)[i] = spElementCreate(source->list[i]->index,source->list[i]->value);
	}
	return res;
}

void spBPQueueDestroy(SPBPQueue* source){
	if (source == NULL) return;
	spBPQueueClear(source);
	free(source->list);
	free(source);
	return;
}

void spBPQueueClear(SPBPQueue* source){
	if (source == NULL) return;
	for (int i=0; i<source->size; i++){
		free(source->list[i]);
	}
	source->size=0;
}

//contract - -1 if null
int spBPQueueSize(SPBPQueue* source){
	if (source == NULL) return -1;
	return source->size;
}

//contract - -1 if null
int spBPQueueGetMaxSize(SPBPQueue* source){
	if (source == NULL) return -1;
	return source->maxSize;
}


SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value){
	if (source == NULL || value < 0) return SP_BPQUEUE_INVALID_ARGUMENT;

	bool full = spBPQueueIsFull(source);
	if (full && (value>spBPQueueMaxValue(source) || (value==spBPQueueMaxValue(source)&&index>=spBPQueueMaxValueIndex(source))))
			return SP_BPQUEUE_FULL;

	BPQueueElement* new_elem = spElementCreate(index,value);
	if (new_elem == NULL) return SP_BPQUEUE_OUT_OF_MEMORY;

	int i; 						// i will be the right index to insert the new element
	if (full){ 					//move from left to right, moving elements to make space
		free(source->list[0]); 	//delete max element in queue;
		for(i = 0; (i < (source->size-1)) && ( (value < (source->list[i+1]->value)) ||
											   ( (value==(source->list[i+1]->value)) &&
												 (index<(source->list[i+1]->index)) 	) )
			; i++){
			source->list[i] = source->list[i+1];
		}
	}
	else{ 						//move from right to left, moving elements to make space;
		for (i=(source->size); (i>0) && ( (value > source->list[i-1]->value) ||
										  ( (value==source->list[i-1]->value) &&
											(index>(source->list[i-1]->index)) 		) )
			; i--){
			source->list[i] = source->list[i-1];
		}
		source->size++;
	}
	//now the i_th place in the list is the right place to insert the new element
	source->list[i] = new_elem;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source){
	if (source == NULL) return SP_BPQUEUE_INVALID_ARGUMENT;
	if (source->size == 0) return SP_BPQUEUE_EMPTY;
	free(source->list[--(source->size)]); //free space and decrease size;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res){
	if(source == NULL || res == NULL) return SP_BPQUEUE_INVALID_ARGUMENT;
	if(spBPQueueIsEmpty(source)) return SP_BPQUEUE_EMPTY;
	BPQueueElement* elem = source->list[source->size-1];
	res->index = elem->index;
	res->value = elem->value;
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res){
	if(source == NULL || res == NULL) return SP_BPQUEUE_INVALID_ARGUMENT;
	if(spBPQueueIsEmpty(source)) return SP_BPQUEUE_EMPTY;
	BPQueueElement* elem = source->list[0];
	res->index = elem->index;
	res->value = elem->value;
	return SP_BPQUEUE_SUCCESS;
}


double spBPQueueMinValue(SPBPQueue* source){
	assert (source!=NULL);
	if (source->size==0){
		return -1;
	}
	return source->list[(source->size-1)]->value;
}

double spBPQueueMaxValue(SPBPQueue* source){
	assert (source!=NULL);
	if (source->size==0){
		return -1;
	}
	return source->list[0]->value;
}
int spBPQueueMaxValueIndex(SPBPQueue* source){
	assert (source!=NULL);
	if (source->size==0){
		return -1;
	}
	return source->list[0]->index;
}

bool spBPQueueIsEmpty(SPBPQueue* source){
	assert (source!=NULL);
	return (source->size == 0);
}

bool spBPQueueIsFull(SPBPQueue* source){
	assert (source!=NULL);
	return (source->size == source->maxSize);
}
