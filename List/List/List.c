#include "List.h"

typedef struct _TNode
{
	void* data;
	struct _TNode *next;
} Node, *PNode;

typedef struct _TList
{
	PNode node;
	PNode lastNode;
	int length;
	bool isEmpty;
	int dataSize;
	ReleaseParams releaseParams;
	DataConstructor dataConstructor;
	DataDestructor dataDestructor;
	DataCopy dataCopy;
} List;

PList ListConstructor(DataConstructor dataConstructor, DataDestructor dataDestructor, DataCopy dataCopy, int dataSize)
{
	PList result = calloc(1, sizeof(List));
	if (result != NULL)
	{
		result->isEmpty = true;
		result->dataSize = dataSize;
		result->dataConstructor = (dataConstructor == NULL ? malloc : dataConstructor);
		result->dataDestructor = (dataDestructor == NULL ? free : dataDestructor);
		result->dataCopy = (dataCopy == NULL ? memcpy_s : dataCopy);
	}
	return result;
}

bool ListIsEmpty(PList list)
{
	return list->isEmpty;
}

int ListGetLength(PList list)
{
	return list->length;
}

int ListGetDataSize(PList list)
{
	return list->dataSize;
}

PNode GetNode(PList list, void* data)
{
	PNode result = calloc(1, sizeof(Node));
	if (result == NULL)
	{
		return NULL;
	}
	
	result->data = list->dataConstructor(data);
	//if data construction failed
	if (result->data == NULL)
	{
		free(result);
		return NULL;
	}

	//if data copy failed
	if (list->dataCopy(result->data, list->dataSize, data, list->dataSize))
	{
		list->dataDestructor(result->data);
		free(result);
		return NULL;
	}

	return result;
}

bool ListPushBack(PList list, void *data)
{
	PNode newItem = GetNode(list, data);
	if (newItem == NULL)
		return NULL;

	if (list->isEmpty)
	{
		list->isEmpty = false;
		list->node = newItem;
	}
	else
	{
		list->lastNode->next = newItem;
	}

	list->lastNode = newItem;
	++list->length;

	return true;
}