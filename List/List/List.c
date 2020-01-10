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
	size_t dataSize;
	DataConstructor dataConstructor;
	DataDestructor dataDestructor;
} List;

void* ListAlloc(void* data, size_t dataSize);
PNode GetNode(PList list, void* data);

void* ListAlloc(void* data, size_t dataSize)
{
	UNREFERENCED_PARAMETER(data);
	return malloc(dataSize);
}


PList ListConstructor(DataConstructor dataConstructor, DataDestructor dataDestructor, size_t dataSize)
{
	PList result = calloc(1, sizeof(List));
	if (result != NULL)
	{
		result->isEmpty = true;
		result->dataSize = dataSize;
		result->dataConstructor = (dataConstructor == NULL ? ListAlloc : dataConstructor);
		result->dataDestructor = (dataDestructor == NULL ? free : dataDestructor);
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
	
	result->data = list->dataConstructor(data, list->dataSize);
	//if data construction failed
	if (result->data == NULL)
	{
		free(result);
		return NULL;
	}

	return result;
}

bool ListPushBack(PList list, void *data)
{
	PNode newItem = GetNode(list, data);
	if (newItem == NULL)
		return false;

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

bool ListPushFront(PList list, void* data)
{
	PNode newItem = GetNode(list, data);
	if (newItem == NULL)
		return false;

	if (list->isEmpty)
	{
		list->isEmpty = false;
		list->lastNode = newItem;
	}
	else
	{
		newItem->next = list->node;
	}

	list->node = newItem;
	++list->length;

	return true;
}


void ListOutput(PList list, OutputFunction outputFunction)
{
	if (!list->isEmpty)
	{
		for (PNode item = list->node; item != NULL; item = item->next)
		{
			outputFunction(item->data);
		}
	}
}