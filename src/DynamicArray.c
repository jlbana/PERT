#include "DynamicArray.h"

DynamicArray* createArray(u32 size)
{
    u32 psize                = size * sizeof(void*);
    /*** || Array  || An implementation of a dynamic array,
         | Element1 | given an initial block-size:
         | Element2 | Construct a singly-linked list which
              ..      points to the next block if the curr
         | Elementn | -ent is exhausted.
         | Next     |
    ***/
    DynamicArray* array      = malloc(sizeof(*array));
    array->size = array->left= size;
    array->array             = malloc(psize);
    array->next              = NULL;
    memset(array->array, 0, psize);
    return array;
}

BlockInfo partialArray(DynamicArray* array)
{
    /*** Index = 0         Index = Array->size
         || Array ||    *> || Array ||
             ...       /       ...
         | Next    | *-    | NULL    |
         Searches in the singly-linked list for the first
         partial array, if not available, the tail is ret
         -urned alongside its equivalent index.
    ***/
    BlockInfo block = {array, 0};
    while (! block.array->left && block.array->next)
    {
        block.index += array->size;
        block.array  = block.array->next;
    }
    return block;
}

DynamicArray* extendArray(DynamicArray* array)
{
    /*** A extend function that searches for an
         empty-spot, or creates an empty block.
         By calling partialArray(), we get
         either a partial-block or tail;
         TAIL:     (->next, ->left) = (NULL, 0)
         PARTIAL:  (->next, ->left) = (?,   !0)
         We can differentiate the two cases by
         using ->left in a reliable-way.
    ***/
    array = partialArray(array).array;

    if (array->left)
        return array;
    return array->next = createArray(array->size);
}

void* appendArray(DynamicArray* array, void* element)
{
    /*** Find or create a block if the current
         is full, before appending the element;
         / Block  \     / Block   \
         | Left=0 |  *- | Element |
         | Next   | /   | NULL    |
    ***/
    if (! array->left)
        array = extendArray(array);

    array->array[
        array->size - array->left--
    ] = element;
    return element;
}

DynamicArray* identifyBlock(DynamicArray* array, u32 index)
{
    /*** Get element at a given index, we identify the
         block by constantly dividing with block size.
         If the index is out-of-bounds, return NULL.

         / Block    \      / Block      \
         | Element1 |      | Elementn+1 | )> Second block,
            ..          *-      ..           Index=0
         | Elementn |  /   | Elementn+n |
         | Next     | *    | NULL       |
    ***/
    if (! array)
        return NULL;

    while (index > array->size - 1)
    {
        index   /= array->size;
        if (! array->next)
            return NULL;
        array    = array->next;
    }
    return array;
}

void setAt(DynamicArray* array, u32 index, void* value)
{
    /*** Overwrites a value in a specific index,
         returns directly for OOB accesses.
    ***/
    array = identifyBlock(array, index);
    if (! array)
        return;
    array->array[index % array->size] = value;
}

void *getAt(DynamicArray* array, u32 index)
{
    /*** Fetches the element at a given
         index in the array.
    ***/
    array = identifyBlock(array, index);
    if (! array)
        return NULL;
    return array->array[index % array->size];
}

u32 getSize(DynamicArray* array)
{
    /*** Calculates the number of free slots
         in the array.
    ***/
    while(array->next)
        array = array->next;
    return array->size - array->left;
}

void destroyArray(DynamicArray* array)
{
    /*** Destroy whole array by deleting all of
         its blocks, and finally the main conta
         -iner.
         /// Container // Block 1 // Block 2 ..
             3            2          1
    ***/
    if (! array)
        return;
    destroyArray(array->next);
    free(array->array);
    free(array);
}
