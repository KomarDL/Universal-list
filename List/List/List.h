#ifndef UNIVERSAL_LIST
#define UNIVERSAL_LIST

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <windows.h>

typedef void(*OutputFunction)(void *data);
typedef bool(*Comparator)(void *first, void *second);
typedef void*(*DataConstructor)(void *data, size_t sizeofData);
typedef void(*DataDestructor)(void *data);

typedef struct _TList *PList;

PList ListConstructor(DataConstructor dataConstructor, DataDestructor dataDestructor, size_t dataSize);

bool ListIsEmpty(PList list);

int ListGetLength(PList list);

int ListGetDataSize(PList list);

bool ListPushBack(PList list, void *data);

bool ListPushFront(PList list, void* data);

bool ListPushAt(PList list, void* data, int index);

void* ListPopBack(PList list);

void* ListPopFront(PList list);

void* ListPopAt(PList list, int index);

void ListOutput(PList list, OutputFunction outputFunction);

void ListSort(PList list, Comparator cmp);

void ListClear(PList list);

void ListRelease(PList* list);

void ListReverse(PList pl);

void* ListGetFront(PList list);

void* ListGetBack(PList list);

void* ListGetAt(PList list, int index);

PList ListCopy(PList list);

bool ListAppend(PList dstList, PList* srcList, bool deleteSrc);

#endif // !UNIVERSAL_LIST

