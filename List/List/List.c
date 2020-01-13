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
	void* result = malloc(dataSize);
	if (result == NULL)
		return NULL;
	if (memcpy_s(result, dataSize, data, dataSize))
	{
		free(result);
		result = NULL;
	}
	return result;
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

bool ListPushAt(PList list, void* data, int index)
{
	index = abs(index);

	if (index == 0)
		return ListPushFront(list, data);

	if (index >= list->length + 1)
		return ListPushBack(list, data);

	PNode newItem = GetNode(list, data);
	if (newItem == NULL)
		return false;

	PNode tmp = list->node;
	for (int i = 1; i < index - 1; ++i)
	{
		tmp = tmp->next;
	}
	newItem->next = tmp->next;
	tmp->next = newItem;
	++list->length;

	return true;
}

void* ListPopBack(PList list)
{
	void* result = NULL;

	if (list->isEmpty)
		return result;

	result = list->lastNode->data;

	--list->length;
	PNode preLast = NULL;
	if (list->length > 1)
	{
		for (preLast = list->node; preLast->next != list->lastNode; preLast = preLast->next);
		free(list->lastNode);
		list->lastNode = preLast;
		list->lastNode->next = NULL;
	}
	else
	{
		free(list->lastNode);
		list->node = NULL;
		list->lastNode = NULL;
		list->isEmpty = true;
	}

	return result;
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