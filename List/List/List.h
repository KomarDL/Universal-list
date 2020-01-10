#ifndef UNIVERSAL_LIST
#define UNIVERSAL_LIST

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <windows.h>

typedef void(*OutputFunction)(void *data);
typedef bool(*Comparator)(void *first, void *second);
typedef void*(*DataConstructor)(void *params);
typedef void(*DataDestructor)(void *data);
typedef errno_t(*DataCopy)(void *dest, size_t destSize, const void *src, size_t count);

typedef struct _TList *PList;

PList ListConstructor(DataConstructor dataConstructor, DataDestructor dataDestructor, DataCopy dataCopy, int dataSize);

bool ListIsEmpty(PList list);

int ListGetLength(PList list);

int ListGetDataSize(PList list);

bool ListPushBack(PList list, void *data);

#endif // !UNIVERSAL_LIST

