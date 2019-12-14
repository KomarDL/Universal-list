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