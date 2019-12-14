#ifndef UNIVERSAL_LIST
#define UNIVERSAL_LIST

#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

typedef void(*OutputFunction)(void *data);
typedef bool(*Comparator)(void *first, void *second);
typedef void*(*GetParams)(void *data);
typedef void(*ReleaseParams)(void *params);
typedef void*(*DataConstructor)(void *params);
typedef void(*DataDestructor)(void *data);
typedef errno_t(*DataCopy)(void *dest, size_t destSize, const void *src, size_t count);

#endif // !UNIVERSAL_LIST

