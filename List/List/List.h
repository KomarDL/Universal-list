#ifndef UNIVERSAL_LIST
#define UNIVERSAL_LIST

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <WinError.h>
#include <errhandlingapi.h>

typedef void(*OutputFunction)(void *data);
typedef bool(*Comparator)(void *first, void *second);
typedef void*(*GetParams)(void *data);
typedef void(*ReleaseParams)(void *params);
typedef void*(*DataConstructor)(void *params);
typedef void(*DataDestructor)(void *data);
typedef errno_t(*DataCopy)(void *dest, size_t destSize, const void *src, size_t count);

typedef struct _TList *PList;

PList ListConstructor(DataConstructor dataConstructor, DataDestructor dataDestructor,
					GetParams getParams, ReleaseParams releaseParams,
					DataCopy dataCopy, int dataSize);

#endif // !UNIVERSAL_LIST

