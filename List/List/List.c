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
	GetParams getParams;
	ReleaseParams releaseParams;
	DataConstructor dataConstructor;
	DataDestructor dataDestructor;
	DataCopy dataCopy;
} List;

PList ListConstructor(DataConstructor dataConstructor, DataDestructor dataDestructor,
					GetParams getParams, ReleaseParams releaseParams,
					DataCopy dataCopy, int dataSize)
{
	if (getParams == NULL || releaseParams == NULL)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return NULL;
	}

	PList result = calloc(1, sizeof(List));
	if (result != NULL)
	{
		result->isEmpty = true;
		result->dataSize = dataSize;
		result->dataConstructor = (dataConstructor == NULL ? malloc : dataConstructor);
		result->dataDestructor = (dataDestructor == NULL ? free : dataDestructor);
		result->dataCopy = (dataCopy == NULL ? memcpy_s : dataCopy);
		result->getParams = getParams;
		result->releaseParams = releaseParams;
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

bool ListPushBack(PList list, void *data)
{
	PNode newItem = calloc(1, sizeof(Node));
	if (newItem == NULL)
	{
		return false;
	}

	void *params = list->getParams(data);
	//if getParams failed
	if (params == NULL)
	{
		free(newItem);
		return false;
	}

	newItem->data = list->dataConstructor(params);
	//if data construction failed
	if (newItem->data == NULL)
	{
		list->releaseParams(params);
		free(newItem);
		return false;
	}

	//if data copy failed
	if (list->dataCopy(newItem->data, list->dataSize, data, list->dataSize))
	{
		list->dataDestructor(newItem->data);
		list->releaseParams(params);
		free(newItem);
		return false;
	}

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