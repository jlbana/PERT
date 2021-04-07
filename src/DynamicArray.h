#ifndef _H_DYNAMICARRAY
#define _H_DYNAMICARRAY

#include <stdlib.h>
#include <string.h>
#include "Types.h"

struct _DynamicArray
{
    u32     size;
    u32     left;
    void**  array;
    struct _DynamicArray* next;
};

typedef struct _DynamicArray DynamicArray;

typedef struct {
    DynamicArray* array;
    u32           index;
} BlockInfo;

DynamicArray* createArray (u32);
void*         appendArray (DynamicArray*, void*);
void*         getAt       (DynamicArray*, u32);
void          setAt       (DynamicArray*, u32, void*);
u32           getSize     (DynamicArray*);
void          destroyArray(DynamicArray*);

#endif
