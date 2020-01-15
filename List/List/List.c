#include "List.h"

typedef struct _TNode
{
	void* data;
	struct _TNode *next;
} Node, *PNode;

#define STACK_LENGTH 64
typedef struct _TSortStack
{
	int level;
	PNode listItem;
} SortStack, * PSortStack;

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
PNode Merge(PList list, PNode first, PNode second, Comparator cmp);

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

	if (index >= list->length)
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

void* ListPopFront(PList list)
{
	void* result = NULL;

	if (list->isEmpty)
		return result;

	result = list->node->data;
	PNode tmp = list->node;
	list->node = list->node->next;
	free(tmp);
	if (--list->length == 0)
	{
		list->isEmpty = true;
		list->lastNode = NULL;
	}

	return result;
}

void* ListPopAt(PList list, int index)
{
	void* result = NULL;

	index = abs(index);

	if (index == 0)
		return ListPopFront(list);

	if (index >= list->length - 1)
		return ListPopBack(list);

	PNode tmp = list->node;
	for (int i = 0; i < index - 1; ++i)
	{
		tmp = tmp->next;
	}
	PNode forDispose = tmp->next;
	tmp->next = forDispose->next;
	result = forDispose->data;
	free(forDispose);
	--list->length;

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

void ListRelease(PList* list)
{
	do
	{
		void* data = ListPopFront(*list);
		if (data != NULL)
		{
			(*list)->dataDestructor(data, (*list)->dataSize);
		}
	} while (!(*list)->isEmpty);
	free(*list);
	*list = NULL;
}

void ListClear(PList list)
{
	while (!list->isEmpty)
	{
		void* data = ListPopFront(list);
		if (data != NULL)
		{
			list->dataDestructor(data, list->dataSize);
		}
	}
}

PNode Merge(PList list, PNode first, PNode second, Comparator cmp)
{
	PNode currItem = NULL;
	if (cmp(first->data, second->data))
	{
		currItem = second;
		second = second->next;
	}
	else
	{
		currItem = first;
		first = first->next;
	}

	PNode result = currItem;

	while ((first != NULL) && (second != NULL))
	{
		PNode tmp = NULL;
		if (cmp(first->data, second->data))
		{
			currItem->next = second;
			currItem = second;
			second = second->next;
		}
		else
		{
			currItem->next = first;
			currItem = first;
			first = first->next;
		}

	}

	if (first != NULL)
	{
		currItem->next = first;
	}
	else
	{
		currItem->next = second;
	}

	while (currItem->next != NULL)
	{
		currItem = currItem->next;
	}
	list->lastNode = currItem;

	return result;
}

void ListSort(PList list, Comparator cmp)
{
	if (list->length < 2)
	{
		return;
	}
	SortStack stack[STACK_LENGTH] = { 0 };
	//memset(stack, ~0, sizeof(SortStack) * STACK_LENGTH);
	int i = 0;
	PNode currentItem = list->node;
	while (currentItem != NULL)
	{
		stack[i].level = 1;
		stack[i].listItem = currentItem;
		currentItem = currentItem->next;
		stack[i].listItem->next = NULL;
		++i;
		while ((i > 1) && (stack[i - 1].level == stack[i - 2].level))
		{

			stack[i - 2].listItem = Merge(list, stack[i - 2].listItem, stack[i - 1].listItem, cmp);
			++stack[i - 2].level;
			--i;
		}
	}
	while (i > 1)
	{
		stack[i - 2].listItem = Merge(list, stack[i - 2].listItem, stack[i - 1].listItem, cmp);
		++stack[i - 2].level;
		--i;
	}
	if (i > 0)
	{
		list->node = stack[0].listItem;
	}
}